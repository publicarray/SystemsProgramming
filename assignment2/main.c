/**
 * @file main.c
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief Multi-threaded server, and client process for factorisation.
 * @mainpage Multi-threaded factorisation program
 * <h2><a href="md_readme.html">Install Instructions</a></h2>
 * Complete documentation: main.c, lib.c, lib.h
 * <p>
 * <a href="files.html">All Files</a> | <a href="annotated.html">Data Structures</a>
 */

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
#include "Request.h"
#include "JobQueue.h"
#include "Job.h"
#include "lib.h"

/** @brief Maximum concurrent jobs that are allowed. Also sets the number of slots. */
#define numConcurrentJobs 10
/** @brief The number of times to do a bit-shift on the input number. */
#define numBitShifts 32
/** @brief The default number of threads in the tread pool */
#define numOfThreads 32
/** @brief Cripple threads and slow them down, useful to see the progress %. (in ms) */
#define slowThread 0.3f

Semaphore jobSemaphore;
Mutex jobQMutex, progressMutex;
JobQueue jobQueue;
int shmid, numThreads;
void * sharedMem = NULL;
int isClean = 0;/**< @brief Keep track if everything has been cleaned-up and freed. */
char *clientflag = NULL, *serverflag = NULL, *progress = NULL;
i32 *number = NULL, *slot = NULL;
Mutex *mutexes = NULL;
int *progressArr[numConcurrentJobs];
int testMode = 0;

/**
 * @brief Removes all objects and frees memory.
 */
void cleanup() {
    if (isClean == 0) {
        puts("\nCleaning up ...");
        jobQueue.destroy(&jobQueue);
        jobQMutex.free(&jobQMutex);
        jobSemaphore.free(&jobSemaphore);
        freeSharedMem(shmid);
        free(mutexes);
        for (int i = 0; i < numConcurrentJobs; i++) {
            free(progressArr[i]);
        }
        isClean = 1;
        puts("\n -- See you later!");
    }
}

/**
 * @brief Called when program quits, cleans up all objects and closes all child processes
 * @param sig The signal that causes the program to exit. e.g. INT or QUIT
 */
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

/**
 * @brief Find the next available slot in the array.
 * @param  slotsInUse The array to search.
 * @return            The first index or slot number that is free.
 */
int findFreeSlot(int *slotsInUse) {
    for (int i = 0; i < numConcurrentJobs; i++) {
        if (slotsInUse[i] == 0) {
            return i;
        }
    }
    return -1; // all slots are in use
}

/**
 * @brief Save the value to the shared memory slot so that the client can read it. [Thread safe]
 * @param slotNum The slot number to write to
 * @param value   The 32 number to write to the slot
 */
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

/**
 * @brief Reset the progress array for a specific slot.
 * @param slot The slot number to reset.
 */
void resetProgress(int slot) {
    // reset progressArray
    for (int i = 0; i < numThreads; i++) {
        progressArr[slot][i] = -1;
    }
}

/**
 * @brief Read the progress from all threads working at a specific slot and
 * calculate the total progress for the slot.
 * @param  slot The slot number to read from
 * @return      The progress as a percentage
 */
int getProgress(int slot) { // The number may fluctuate because any thread my work for any job/slot
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

/**
 * @brief Display a progress bar as a percentage in the terminal without new lines.
 * @param progress The current progress out of 100.
 */
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

/**
 * @brief The function running in the Threads.
 *
 * The threads look for jobs to work on and
 * factorise the number given in the job. The result and progress is written to
 * global variables. The threads can also run in test mode.
 * @param  threadId The Thread number
 * @return          nothing
 */
void * worker (void * threadId) {
    int id = (int) threadId;
    printf("Thread #%d started.\n", id);
    while (1) {
        int hasJob = 0;
        Job j;
        // printf("Thread # %d sleeping\n", id);
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
            // printf("Working job # %d\n", j.slot);
            progressArr[j.slot][id] = 0;

            if (testMode) {
                for (int i = 0; i < 10; ++i) { // output ten numbers
                    // printf("thread:%d set:%d slot:%d result:%d\n", id, j.data, j.slot, (id * 10) + i);
                    writeToSlot(j.slot, (id * 10) + i);
                }
            } else {
                // Factorise()
                for (i32 i = 2; i*i <= j.data; i++) {
                     // Cripple threads so that the progress bar shows
                    if (slowThread > 0.0f) {
                        tsleep(slowThread); // slowdown the thread
                    }

                    if (j.data % i == 0) {
                       // printf("found factor: %u\n", i);
                        writeToSlot(j.slot, i); // factor
    //                    printf("found factor: %u\n", j.data/i);
                        writeToSlot(j.slot, j.data/i); // the result of (n/i) i.e the other factor
                    }
                    progressMutex.lock(&progressMutex);
                    progressArr[j.slot][id] = (100 * i*i) / j.data; // update progress Array
                    progressMutex.unlock(&progressMutex);
    //                printf("progress: %d \n", progressArr[j.slot][id]);
                }
            }

            progressMutex.lock(&progressMutex);
            progressArr[j.slot][id] = 100; // update progress Array
            progressMutex.unlock(&progressMutex);
            // printf("Thread # %d is DONE: \n", id);
            hasJob = 0;
        }
    }
    return 0;
}

/**
 * @brief Initialises the server and runs the client.
 *
 * The server process runs a thread pool and
 * waits for commands from the client. The server sends user commands to
 * the server and passes responses back to the user.
 * @param  argc Number of arguments given
 * @param  argv The arguments from the user, the number of threads to spawn
 * @return      Exit code
 */
int main(int argc, char const *argv[]) {
    signal(SIGQUIT, terminate); // clean-up
    int i;
    jobSemaphore = newSemaphore(0);
    jobQMutex = newMutex();
    progressMutex = newMutex();
    jobQueue = newJobQueue();
    shmid = newSharedMem(1 + 10 + sizeof(i32) + (sizeof(i32)*10) + 10);
    sharedMem = getSharedMem(shmid);

    clientflag = sharedMem;
    serverflag = sharedMem + 1;
    number = sharedMem + 1 + 10;
    slot = sharedMem + 1 + 10 + sizeof(i32);
    progress = sharedMem + 1 + 10 + sizeof(i32) + sizeof(i32) * 10;

    *clientflag = '0'; // set initial values
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
        printf("Child process initialised\n");
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

        puts("Enter number to factor: ");
        while (1) {
            if (*clientflag == '1') { // if there is data to read
                i32 num = *number; // get data
                if (num == 0) { // if test mode
                    testMode = 1;
                    printf("\nReceived: TEST MODE\n");
                    for (int i = 0; i < 3; i++) { // simulate 3 user requests
                        int slot = -1;
                        if ((slot = findFreeSlot(slotsInUse)) == -1) {
                            *clientflag = '2'; // signal that the server is busy
                            break;
                        }
                        for (int j = 0; j < 10; j++) { // create a set of 10 threads/jobs
                            slot = i;
                            slotsInUse[slot] = 1;
                            *number = slot; //slot; // TODO: this will set 3 different values.
                            *clientflag = '0';
                            jobQMutex.lock(&jobQMutex);
                            Job job = newJob(slot, j);
                            jobQueue.push(&jobQueue, job);
                            jobQMutex.unlock(&jobQMutex);
                            jobSemaphore.signalX(&jobSemaphore, 10); // signal 10 threads to work
                        }
                    }
                } else {
                    testMode = 0;
                    int slot = -1;
                    if ((slot = findFreeSlot(slotsInUse)) == -1) {
                        *clientflag = '2'; // signal that the server is busy
                    } else  {
                        printf("\nReceived: %u set slot id to %d\n", num, slot);
                        slotsInUse[slot] = 1;
                        *number = slot;
                        *clientflag = '0';

                        jobQMutex.lock(&jobQMutex);
                        for (i = 0; i < numBitShifts; i++) {
                            Job j = newJob(slot, num);
                            num = rotr32(num, 1);
                            jobQueue.push(&jobQueue, j);
                        }
                        jobQMutex.unlock(&jobQMutex);
                        jobSemaphore.signalX(&jobSemaphore, numBitShifts); // signal threads to work
                    }
                }
            }

            // progress
            for (i = 0; i < numConcurrentJobs; i++) {
                if (slotsInUse[i] == 1) {
                    int slotProgress = getProgress(i);
                    progress[i] = slotProgress; // save to shared memory
//                    printf("Slot # %d -> %d %%\n", i, slotProgress);
                    if (slotProgress >= 100) {
                        serverflag[i] = '2'; // notify client that the job is done
                        slotsInUse[i] = 0;
                        resetProgress(i);
                    }
                }
            }

            tsleep(3); // reduce CPU usage when idle
        }

        return 0;
    }
    printf("Child process has a pid of %d \n", pid);
    // parent process ("the client")
    signal(SIGINT, terminate); // clean-up [Control + C]

    int bufferSize = 10000;
    char userBuffer[bufferSize];
    Request requests[numConcurrentJobs];
    int outstandingJobs = 0;
    struct timespec startTime = getTime();
    float duration = 0.0f;
    int showedProgress = 0;

    while (1) {
        duration = getTimeLapsed(startTime);
        if (outstandingJobs > 0 && duration >= 0.5f && !testMode) { // 0.5s = 500 milliseconds
            for (int i = 0; i < 10 + 15*outstandingJobs; i++) {
                printf("\b");
            }
            printf("Progress: ");
                // progresDisplay(progress[0]);
        }

        // read data from slots
        for (i = 0; i < numConcurrentJobs; i++) {
            int temp = 0;
            if (outstandingJobs > 0 && duration >= 0.5f && (temp = progress[i]) != 'x' && !testMode) {
                printf("%*d:%*d%% ", 2, i, 3, temp);
                showedProgress = 1;
            }

            if (serverflag[i] == '1') { // data to read
                if (showedProgress) {
                    printf("\n");
                    showedProgress = 0;
                }
                printf("Slot: %d, Number: %u, Factor: %u \n", i, requests[i].number, slot[i]);
                serverflag[i] = '0';
                startTime = getTime();
            } else if (serverflag[i] == '2') { // finished job
                if (showedProgress) {
                    printf("\n");
                    showedProgress = 0;
                }
                printf(BLU "Slot # %d is done processing %d in %f seconds" NRM "\n", i, requests[i].number, requests[i].duration(&requests[i]));
                progress[i] = 'x';
                serverflag[i] = '0';
                outstandingJobs--;
                startTime = getTime();
            }
        }
        fflush(0);

        // read data from user
        if (canRead(STDIN_FILENO, 0, 100)) { // if user typed something
            startTime = getTime();
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
                continue; // ignore non number characters
            }

            if (outstandingJobs > 0 && atoi(userBuffer) == 0) {
                printf(RED "Please wait for all jobs to finish first before starting test" NRM "\n");
                continue;
            } else if (atoi(userBuffer) == 0) {
                testMode = 1;
            } else {
                testMode = 0;
            }

            // send data to server
            // TODO: have a time-out and tell the user that the server has not read the data or that there is not data to read
            while(*clientflag != '0'){tsleep(50);} // wait until allowed to write
            i32 temp = *number = atoi(userBuffer); // pass to server
            *clientflag = '1';
            while(*clientflag == '1' && !testMode){tsleep(50);} // wait until the server has read the data
            if (*clientflag == '2') { // all slots are in use, tell user we are busy
                puts("\n" RED "Server is busy!" NRM "\n");
                *clientflag = '0';
            } else if (testMode) {
                requests[0] = newRequest(0);
                requests[1] = newRequest(0);
                requests[2] = newRequest(0);
                outstandingJobs+=3;
            } else {
                int id = *number; // read server reply
                requests[id] = newRequest(temp); // remember which slots have jobs running
                outstandingJobs++;
            }
        }
        tsleep(3); // reduce CPU usage when idle

    }

    wait(NULL); // wait for child to exit
    cleanup();
    return 0;
}
