#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include "Thread.h"
// #include "SharedMemory.h" // do not use, broken
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Semaphore.h"
//#include "Queue.h"
#include "JobQueue.h"
#include "Job.h"
#include "lib.h"

#define numConcurrentJobs 10

Semaphore jobSemaphore, doneSemaphore;
Mutex jobQMutex, clientflagMutex, numberMutex;
JobQueue jobQueue, doneQueue;
int shmid;
void * sharedMem = NULL;
int isClean = 0;
char *clientflag = NULL, *serverflag = NULL, *progress = NULL;
i32 *number = NULL, *slot = NULL;
Mutex *mutexes = NULL;

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
        isClean = 1;
    }
}

void terminate (int sig) {
    // clean up
    cleanup();
    puts("\nShut-down successfully!");
     int pid;
     while ((pid = wait(NULL)) > 0) { // wait for processes to finish
         printf("Stopping Server: %d!\n", pid);
         if (pid == -1 && errno != 10) { // 10 = No child processes
             perror("terminate wait()");
         }
     }
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

void * worker (void * data) {
    puts("thread started ...");
    while (1) {
        int hasJob = 0;
        Job j;

        jobSemaphore.wait(&jobSemaphore); // wait for job
        printf("Woken up\n");
        // we might be woken up at any time
        jobQMutex.lock(&jobQMutex);
        if (!jobQueue.isEmpty(&jobQueue)) {
            printf("Getting job..\n");
            hasJob = 1;
            j = jobQueue.pop(&jobQueue);
        }
        jobQMutex.unlock(&jobQMutex);

        // do job
        if (hasJob) {
            printf("Working job #%d\n", j.id);
            // Factorise()
            for (i32 i = 2; i*i <= j.data; i++) {
                if (j.data % i == 0) {
                    printf("found factor: %u\n", i);
//                    j.output = i;
//                    while (serverflag[j.id] != '0'){tsleep(50);} // wait until slot is free
                    while(1) {
                        mutexes[j.id].lock(&mutexes[j.id]);
                        if (serverflag[j.id] == '0') {
                            slot[j.id] = i;
                            serverflag[j.id] = '1';
                            mutexes[j.id].unlock(&mutexes[j.id]);
                            break;
                        } else {
//                            printf("serverflag #%d -> %c\n", j.id, serverflag[j.id]);
                            mutexes[j.id].unlock(&mutexes[j.id]);
                            tsleep(10);
                        }
                    }
                    
                    // the result of (n/i) i.e the other factor
                    while(1) {
                        mutexes[j.id].lock(&mutexes[j.id]);
                        if (serverflag[j.id] == '0') {
                            slot[j.id] = j.data/i;
                            serverflag[j.id] = '1';
                            mutexes[j.id].unlock(&mutexes[j.id]);
                            break;
                        } else {
//                            printf("serverflag #%d -> %c\n", j.id, serverflag[j.id]);
                            mutexes[j.id].unlock(&mutexes[j.id]);
                            tsleep(10);
                        }
                    }
                    
                    j.progress = (i*i)/j.data;
                    
//                    doneQMutex.lock(&doneQMutex);
//                    doneQueue.push(&doneQueue, &j);
//                    doneQMutex.unlock(&doneQMutex);
//                    doneSemaphore.signal(&doneSemaphore);
//                    slot.print(&slot);
                    // while (!serverflag.getNumP(&serverflag)[j.id]); // bissy waiting
                    // slot.data = i; // == ((i32*)slot.data)[j.id] = i;
                    // slot.getNumP(&slot)[j.id] = i; // == ((i32*)slot.data)[j.id] = i;
//                    slot.print(&slot);

//                    printf("factor: %d\n", i);

                    // serverflag.getNumP(&serverflag)[j.id] = 1;

                    // i = smallest factor found
                    // signal facto found, update shared vars
                    // wait for client to read.
                    // return i; // n/i = result
                }
            }
            
            
            
            // tel the clint that we are done
//            while(1) {
//                mutexes[j.id].lock(&mutexes[j.id]);
//                if (serverflag[j.id] == '0') {
//                    serverflag[j.id] = '2'; // 2 = done
//                    mutexes[j.id].unlock(&mutexes[j.id]);
//                    break;
//                } else {
//                    mutexes[j.id].unlock(&mutexes[j.id]);
//                    tsleep(10);
//                }
//            }
            
            
            printf("Thread is DONE: \n");
            hasJob = 0;
            j.done = 1;
            
//            doneQMutex.lock(&doneQMutex);
//            doneQueue.push(&doneQueue, &j);
//            doneQMutex.unlock(&doneQMutex);
//            doneSemaphore.signal(&doneSemaphore);
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
    for (i = 0; i < 10; i++) {
        serverflag[i] = '0';
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
        
        int slotsInUse[10] = {0};
        
        mutexes = malloc(numConcurrentJobs * sizeof (Mutex));
        for (i = 0; i <= numConcurrentJobs; i++) {
            Mutex m = newMutex();
            mutexes[i] = m;
        }
        
        
        // Thread pool
        int numThreads = 32;

        if (argv[1]) {
            numThreads = atoi(argv[1]);
        }
        
        for (i = 0; i < numThreads; ++i) {
            Thread t = newThread();
            t.startDetached(&t, worker, 0);
        }

        while (1) {
            if (*clientflag == '1') { // wait untill there is data to read
                // get job
                i32 num = *number;
                int id = -1;
                if ((id = findFreeSlot(slotsInUse)) == -1) {
                    printf("server is bussy\n"); // Todo: send info to client.
                } else  {
                    printf("recieved: %u set id to %d\n", num, id);
                    slotsInUse[id] = 1;
                    *number = id;
                    *clientflag = '0';

                    jobQMutex.lock(&jobQMutex);
                    for (i = 0; i < 32; i++) {
                        Job j = newJob(id, num);
                        num = rotr32(num, 1);
                        jobQueue.push(&jobQueue, j);
                    }
                    jobQMutex.unlock(&jobQMutex);
                    jobSemaphore.signalX(&jobSemaphore, 32); // signal threads to work
                }
            }
            tsleep(500);
        }
        
        return 0;
    }
    printf("parent continues execution, child process has a pid of %d \n", pid);
    // parent process ("the client")

    int bufferSize = 10000;
    char userBuffer[bufferSize];
    int arr[numConcurrentJobs];
//    while(1) {
//        mutexes[j.id]->lock(mutexes[j.id]);
//        if (serverflag[j.id] != '0') {
//            slot[j.id] = i;
//            serverflag[j.id] = '1';
//        } else {
//            puts("serverflag is == '1', sleeping");
//            tsleep(50);
//        }
//        mutexes[j.id]->unlock(mutexes[j.id]);
//    }
    
    
     while (1) {

         // read data from slots
         for (i = 0; i < numConcurrentJobs; i++) {
//             tsleep(500);
//             printf("#%d -> %c\n", i, serverflag[i]);
//             mutexes[i].lock(&mutexes[i]);
             if (serverflag[i] == '1') {
                 printf("Slot: %d, OrigNum: %u, Factor: %u \n", i, arr[i], slot[i]);
                 serverflag[i] = '0';
             }
//             mutexes[i].unlock(&mutexes[i]);
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
                 puts("\n -- See you later!");
                 break; // exit loop
             }
//             if () // all slots are in use, tell user we are busy
            // send data to server
            while(*clientflag != '0'){tsleep(50);} // wait until allowed to write
            i32 temp = *number = atoi(userBuffer); // pass to server
            *clientflag = '1';
            while(*clientflag != '0'){tsleep(50);}
            int id = *number; // read server reply
            arr[id] = temp; // remember which slots have jobs running;
        }

    }

    wait(NULL); // wait for child to exit
    cleanup();
    return 0;
}
