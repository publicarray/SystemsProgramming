#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include "Thread.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Semaphore.h"
//#include "Queue.h"
#include "JobQueue.h"
#include "Job.h"
#include "lib.h"

#define numConcurrentJobs 10
#define numBitShifts 32
#define numOfThreads 32

Semaphore jobSemaphore, doneSemaphore;
Mutex jobQMutex, clientflagMutex, numberMutex, progressMutex;
JobQueue jobQueue, doneQueue;
int shmid, numThreads;
void * sharedMem = NULL;
int isClean = 0;
char *clientflag = NULL, *serverflag = NULL, *progress = NULL;
i32 *number = NULL, *slot = NULL;
Mutex *mutexes = NULL;
int *progressArr[numConcurrentJobs];

void cleanup() {
    if (isClean == 0) {
        puts("\nCleaning up ...");
        jobQueue.destroy(&jobQueue);
        doneQueue.destroy(&doneQueue);
        jobQMutex.free(&jobQMutex);
        clientflagMutex.free(&clientflagMutex);
        numberMutex.free(&numberMutex);
        jobSemaphore.free(&jobSemaphore);
        doneSemaphore.free(&doneSemaphore);
        freeSharedMem(shmid);
        free(mutexes);
        for (int i = 0; i < numConcurrentJobs; i++) {
            free(progressArr[i]);
        }
        isClean = 1;
        puts("\n -- See you later!");
    }
}

void terminate (int sig) {
    int pid;
    while ((pid = wait(NULL)) > 0) { // wait for processes to finish
        printf("Stopping child process: %d!\n", pid);
        if (pid == -1 && errno != 10) { // 10 = No child processes
            perror("terminate wait()");
        }
    }
    cleanup();
    exit(0);
}

int findFreeSlot(int *slotsInUse) {
    for (int i = 0; i < numConcurrentJobs; i++) {
        if (slotsInUse[i] == 0) {
            return i;
        }
    }
    return -1; // all slots are in use
}

void writeToSlot(int slotNum, i32 value) {
    while(1) {
        mutexes[slotNum].lock(&mutexes[slotNum]);
        if (serverflag[slotNum] == '0') { // Xcode -> Data race condition (doesn't like arrays)
            slot[slotNum] = value;
            serverflag[slotNum] = '1';
            mutexes[slotNum].unlock(&mutexes[slotNum]);
            break;
        }
        mutexes[slotNum].unlock(&mutexes[slotNum]);
        tsleep(10);
    }
}

void resetProgress(int slot) {
    // reset progressArray
    for (int i = 0; i < 32; i++) {
        progressArr[slot][i] = -1;
    }
}

int getProgress(int slot) {
    if (slot > numConcurrentJobs) {
        printf("getProgress() slot number too high! %d", slot);
        return -1;
    }
    int slotProgress = 0;
    int workingTheads = 0;
    int temp;
    for (int i = 0; i < numThreads; i++) {
        progressMutex.lock(&progressMutex);
        temp = progressArr[slot][i]; // only access the variable once
        progressMutex.unlock(&progressMutex);
        if (temp > -1) {
            slotProgress += temp;
            workingTheads++;
        }
    }

    if (slotProgress == 0 || workingTheads == 0) {
        return 0;
    }
    return slotProgress / workingTheads;
}

void progresDisplay(int progress) {
    int i;

    for (i = 0; i < progress; i++) {
        printf("#");
    }
    for (i = 0; i < progress; i++) {
        printf("\b");
    }
    fflush(0);
}

void * worker (void * threadId) {
    int id = (int) threadId;
    printf("thread #%d started ...\n", id);
    while (1) {
        int hasJob = 0;
        Job j;

        jobSemaphore.wait(&jobSemaphore); // wait for job
        // printf("Thread # %d Woken up\n", id);
        // we might be woken up at any time
        jobQMutex.lock(&jobQMutex);
        if (!jobQueue.isEmpty(&jobQueue)) {
            // printf("Thread # %d Getting job..\n", id);
            hasJob = 1;
            j = jobQueue.pop(&jobQueue);
        }
        jobQMutex.unlock(&jobQMutex);

        // do job
        if (hasJob) {
            // printf("Working job # %d\n", j.id);
            progressArr[j.id][id] = 0;
            // Factorise()
            for (i32 i = 2; i*i <= j.data; i++) {
                if (j.data % i == 0) {
//                    printf("found factor: %u\n", i);
                    writeToSlot(j.id, i); // factor
//                    printf("found factor: %u\n", j.data/i);
                    writeToSlot(j.id, j.data/i); // the result of (n/i) i.e the other factor
                }
                progressArr[j.id][id] = (100 * i*i) / j.data; // update progress Array
//                printf("progress: %d \n", progressArr[j.id][id]);
               tsleep(300);
            }
            progressMutex.lock(&progressMutex);
            progressArr[j.id][id] = 100; // update progress Array
            progressMutex.unlock(&progressMutex);
            // printf("Thread # %d is DONE: \n", id);
            hasJob = 0;
        }
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    signal(SIGINT, terminate); // cleanup [Control + C]
    signal(SIGQUIT, terminate); // cleanup
    int i;
    jobSemaphore = newSemaphore(0);
    doneSemaphore = newSemaphore(0);
    jobQMutex = newMutex();
    clientflagMutex = newMutex();
    progressMutex = newMutex();
    numberMutex = newMutex();
    jobQueue = newJobQueue();
    doneQueue = newJobQueue();
    shmid = newSharedMem(1 + 10 + sizeof(i32) + (sizeof(i32)*10) + 10);
    sharedMem = getSharedMem(shmid);

    clientflag = sharedMem;
    serverflag = sharedMem + 1;
    number = sharedMem + 1 + 10;
    slot = sharedMem + 1 + 10 + sizeof(i32);
    progress = sharedMem + 1 + 10 + sizeof(i32) + sizeof(i32) * 10;

    *clientflag = '0'; // set intial values
    for (i = 0; i < numConcurrentJobs; i++) {
        serverflag[i] = '0';
        progress[i] = 'x';
    }


    int pid = fork();
    if (pid == -1) {
        perror("Error forking a new process.");
        exit(-1);
    }
    if (pid == 0) {
        printf("child started\n");
        // child process ("the server")
        sharedMem = getSharedMem(shmid);

        clientflag = sharedMem;
        serverflag = sharedMem + 1;
        number = sharedMem + 1 + 10;
        slot = sharedMem + 1 + 10 + sizeof(i32);
        progress = sharedMem + 1 + 10 + sizeof(i32) + sizeof(i32) * 10;

        int slotsInUse[numConcurrentJobs] = {0};

        mutexes = malloc((numConcurrentJobs) * sizeof (Mutex));
        for (i = 0; i < numConcurrentJobs; i++) {
            Mutex m = newMutex();
            mutexes[i] = m;
        }


        // Thread pool
        numThreads = numOfThreads;

        if (argv[1]) {
            numThreads = atoi(argv[1]);
        }

        for (i = 0; i < numThreads; ++i) {
            Thread t = newThread();
            t.startDetached(&t, worker, i);
        }

        // initialise progressArr
        for (i = 0; i < numConcurrentJobs; i++) {
            progressArr[i] = malloc(sizeof(int) * numThreads);
            resetProgress(i);
        }

        while (1) {
            if (*clientflag == '1') { // wait until there is data to read
                                      // get job
                i32 num = *number;
                int id = -1;
                if ((id = findFreeSlot(slotsInUse)) == -1) {
                    *clientflag = '2';
                } else  {
                    printf("\nReceived: %u set slot id to %d\n", num, id);
                    slotsInUse[id] = 1;
                    *number = id;
                    *clientflag = '0';

                    jobQMutex.lock(&jobQMutex);
                    for (i = 0; i < numBitShifts; i++) {
                        Job j = newJob(id, num);
                        num = rotr32(num, 1);
                        jobQueue.push(&jobQueue, j);
                    }
                    jobQMutex.unlock(&jobQMutex);
                    jobSemaphore.signalX(&jobSemaphore, numBitShifts); // signal threads to work
                }
            }

            // progress
            for (i = 0; i < numConcurrentJobs; i++) {
                if (slotsInUse[i] == 1) {
                    int slotProgress = getProgress(i);
                    progress[i] = slotProgress; // save to shared memory
//                    printf("Slot # %d -> %d %%\n", i, slotProgress);
                    if (slotProgress >= 100) {
                        serverflag[i] = '2'; // notefy client that the job is done
                        slotsInUse[i] = 0;
//                        resetProgress(i);
                    }
                }
            }

            tsleep(100);
        }

        return 0;
    }
    printf("parent continues execution, child process has a pid of %d \n", pid);
    // parent process ("the client")

    int bufferSize = 10000;
    char userBuffer[bufferSize];
    int origninalNumber[numConcurrentJobs] = {-1};
    int outstandingJobs = 0;
    // int

    while (1) {

        for (int i = 0; i < 10 + 15*outstandingJobs; i++) {
            printf("\b");
        }
        if (outstandingJobs > 0) {

            printf("Progress: ");
            for (i = 0; i < numConcurrentJobs; i++) {
                if (progress[i] != 'x') {
                    printf("%*d:%*d%% ", 2, i, 3, progress[i]);
                }
            }
            fflush(0);
            // printf("\n");
            // progresDisplay(progress[0]);
        }

        // read data from slots
        for (i = 0; i < numConcurrentJobs; i++) {

            if (serverflag[i] == '1') { // data to read
                // printf("Slot: %d, Number: %u, Factor: %u \n", i, origninalNumber[i], slot[i]);
                serverflag[i] = '0';
            } else if (serverflag[i] == '2') { // finished job
                printf("\nSlot # %d is done processing %d\n", i, origninalNumber[i]);
                progress[i] = 'x';
                origninalNumber[i] = -1;
                serverflag[i] = '0';
                outstandingJobs--;
            }
        }


        // read data from user
        if (canRead(STDIN_FILENO, 0, 100)) { // if user typed something
            fgets(userBuffer, sizeof userBuffer, stdin);
            removeNewLine(userBuffer);

            if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
                puts("\nWaiting for threads to quit...");
                kill(pid, SIGKILL);
                //wait(NULL); // wait for child. TODO: actually tell child we are quitting
                cleanup();
                break; // exit loop
            }

            if (isdigitstr(userBuffer) == 0) { // it not a number
                continue; // ignore empty
            }

            //             if () // all slots are in use, tell user we are busy
            // send data to server
            while(*clientflag != '0'){tsleep(50);} // wait until allowed to write
            i32 temp = *number = atoi(userBuffer); // pass to server
            *clientflag = '1';
            while(*clientflag == '1'){tsleep(50);} // wait until the server has read the data
            if (*clientflag == '2') {
                puts("\nServer is busy!");
                *clientflag = '0';
            } else {
                int id = *number; // read server reply
                origninalNumber[id] = temp; // remember which slots have jobs running;
                outstandingJobs++;
            }
        }
        tsleep(2);

    }

    wait(NULL); // wait for child to exit
    cleanup();
    return 0;
}
