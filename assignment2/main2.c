#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

Semaphore jobSemaphore, doneSemaphore;
Mutex jobQMutex, doneQMutex, numberMutex;
JobQueue jobQueue, doneQueue;
int numberId, clientflagId, slotsId, serverflagId;
int isClean = 0;
char *clientflag, *serverflag;
i32 *number, *slot;


void cleanup() {
    if (isClean == 0) {
        puts("\nCleaning up ...");
        jobQueue.destroy(&jobQueue);
        doneQueue.destroy(&doneQueue);
        jobQMutex.free(&jobQMutex);
        doneQMutex.free(&doneQMutex);
        numberMutex.free(&numberMutex);
        jobSemaphore.free(&jobSemaphore);
        doneSemaphore.free(&doneSemaphore);
        freeSharedMem(numberId);
        freeSharedMem(clientflagId);
        freeSharedMem(slotsId);
        freeSharedMem(serverflagId);
        isClean = 1;
    }
}

void terminate (int sig) {
    // clean up
    cleanup();
    puts("\nShut-down successfully!");
    // int pid;
    // while ((pid = wait(NULL)) > 0) { // wait for processes to finish
    //     printf("Stopping Server: %d!\n", pid);
    //     if (pid == -1 && errno != 10) { // 10 = No child processes
    //         perror("terminate wait()");
    //     }
    // }
    exit(0);
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
                    while (serverflag[j.id] != '0'){tsleep(50);} // wait until slot is free
//                    doneQMutex.lock(&doneQMutex);
                    slot[j.id] = i;
                    serverflag[j.id] = '1';
//                    doneQMutex.unlock(&doneQMutex);

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
            printf("DONE: \n");
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
    doneQMutex = newMutex();
    numberMutex = newMutex();
    jobQueue = newJobQueue();
    doneQueue = newJobQueue();
    clientflagId = newSharedMem(sizeof(char));
    serverflagId = newSharedMem(sizeof(char) * 10);
    numberId = newSharedMem(sizeof (i32));
    slotsId = newSharedMem(sizeof (i32) * 10);

    clientflag = getSharedMem(clientflagId);
    serverflag = getSharedMem(serverflagId);
    number = getSharedMem(numberId);
    slot = getSharedMem(slotsId);
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
        clientflag = getSharedMem(clientflagId);
        serverflag = getSharedMem(serverflagId);
        number = getSharedMem(numberId);
        slot = getSharedMem(slotsId);
        int id = 0;

        // Thread pool
        int numThreads = 4;

        if (argv[1]) {
            numThreads = atoi(argv[1]);
        }
        
        for (i = 0; i < numThreads; ++i) {
            Thread t = newThread();
            t.startDetached(&t, worker, 0);
        }

        while (1) {
            while(*clientflag != '1'); // wait untill there is data to read
            // get job
            i32 num = *number;
            printf("recieved: %u \n", num);
            *number = id;
            *clientflag = '0';
            id++;

            jobQMutex.lock(&jobQMutex);
            for (i = 0; i <= 2; i++) {
                Job j = newJob(id, num);
                num = rotr32(num, 1);
                jobQueue.push(&jobQueue, j);
            }
            jobQMutex.unlock(&jobQMutex);
            jobSemaphore.signalX(&jobSemaphore, 2);
        }

        return 0;
    }
    printf("parent continues execution, child process has a pid of %d \n", pid);
    // parent process ("the client")

    int bufferSize = sizeof(i32);
    char userBuffer[bufferSize];
    int arr[10];

     while (1) {

         for (i = 0; i <= 10; i++) {
//             if (arr[i] == 1) {
                 if (serverflag[i] == '1') {
                     printf("Slot: %d, OrigNum: %u, Factor: %u \n", i, 0, slot[i]);
                     serverflag[i] = '0';
                 }
             
//             }
         }
         
         if (canRead(STDIN_FILENO, 0, 5000)) { // if user typed something
             fgets(userBuffer, sizeof userBuffer, stdin);
             removeNewLine(userBuffer);
             if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
                 puts("\nWaiting for threads to quit...");
                 wait(NULL); // wait for child. TODO: actually tell child we are quitting
                 cleanup();                         puts("\n -- See you later!");
                 break; // exit loop
             }
//             if () // all slots are in use, tell user we are busy
             while(*clientflag != '0'){tsleep(50);} // wait untill allowed to write
             *number = atoi(userBuffer); // pass to server
             *clientflag = '1';
             while(*clientflag != '0'){tsleep(50);}
             int id = *number; // read server reply
             arr[id] = 1; // remember which slots have jobs running;
         }

     }

    wait(NULL); // wait for child to exit
    cleanup();
    return 0;
}
