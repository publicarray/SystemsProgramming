// single threaded
// non-blocking
// query the user for 32 bit integers to be processed and will pass each request to the server to process,
// and will immediately request the user for more input numbers or ‘q’ to quit.

// The client and server will communicate using shared memory.
// The client will write data for the server to a shared 32 bit variable called ‘number’.
// The server will write data for the client to a shared array of 32 bit variables called a ‘slot’
// that is 10 elements long. Each element in the array (slot) will correspond to an individual client
// query so only a maximum of 10 queries can be outstanding at any time. This means that any subsequent
// queries will be blocked until one of the 10 outstanding queries completes, at which times its slot can
// be reused by the server for its response to the new query.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/types.h>
#include "Thread.h"
#include "SharedMemory.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Semaphore.h"
#include "Queue.h"
#include "lib.h"
// #include "../../01StringAndList/String.h"

// #define BUFFERLEN 200
// #define SHARED_MEM_SIZE 1024 // 1k

// void* func(void* num) {
//     // int temp = rotr32( *(i32*)args, 1);
//     // int temp = (*(int *)args)++;
//     // i32 temp =  *(i32*) args;
//     // *(int32_t *)args += 1;
//     // *(i32*) args = temp;
//     printf("thread got: %d\n", *(i32*) num);
//     return (void*) factorise(*(i32*) num);
//     // return NULL;

// }

Semaphore jobSemaphore, doneSemaphore;
Mutex jobQMutex, doneQMutex;
Queue jobQueue, doneQueue;
SharedMemory number, clientflag, slots, serverflag;
// shared vars
// i32 number;
// int clientflag = 0;
// i32 slots[10] = {0,0,0,0,0,0,0,0,0,0};
// int serverflag[10] = {0,0,0,0,0,0,0,0,0,0};

typedef struct {
    int id;
    int data;
    int output;
} Job;

void initJob(Job *self, int id, int data) {
    self->id = id;
    self->data = data;
    self->output = 0;
}

void printJob(Job *self) {
    printf("job #%d, data:%d, output:%d\n", self->id, self->data, self->output);
}



void terminate (int sig) {
    // clean up
    jobQueue.destroy(&jobQueue);
    doneQueue.destroy(&doneQueue);
    jobQMutex.free(&jobQMutex);
    doneQMutex.free(&doneQMutex);
    jobSemaphore.free(&jobSemaphore);
    doneSemaphore.free(&doneSemaphore);
    number.free(&number);
    clientflag.free(&clientflag);
    slots.free(&slots);
    serverflag.free(&serverflag);
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

void* worker (void* num) {
    while (1) {
        //get a job
        int hasJob = 0;
        Job j;

        jobSemaphore.wait(&jobSemaphore); // wait for job
        printf("Woken up\n");
        // we might be woken up at any time
        jobQMutex.lock(&jobQMutex);
        if (!jobQueue.isEmpty(&jobQueue)) {
            printf("Getting job..\n");
            hasJob = 1;
            j = *(Job*) jobQueue.pop(&jobQueue);
        }
        jobQMutex.unlock(&jobQMutex);

        // do job
        if (hasJob) {
            printf("Working job #%d\n", j.id);

            // Factorise()
            for (i32 i = 2; i*i <= j.data; i++) {
                if (j.data % i == 0) {
                    printf("factor: %d\n", i);
                    slots.print(&slots);
                    // while (!serverflag.getNumP(&serverflag)[j.id]); // bissy waiting
                    // slots.data = i; // == ((i32*)slots.data)[j.id] = i;
                    // slots.getNumP(&slots)[j.id] = i; // == ((i32*)slots.data)[j.id] = i;
                    slots.print(&slots);

                    printf("factor: %d\n", i);

                    // serverflag.getNumP(&serverflag)[j.id] = 1;

                    // i = smallest factor found
                    // signal facto found, update shared vars
                    // wait for client to read.
                    // return i; // n/i = result
                }
            }


            // j.output = slots.getNumP(&slots)[j.id]; // largest factor atm
            tsleep(200);
            doneQMutex.lock(&doneQMutex);
            doneQueue.push(&doneQueue, &j);
            doneQMutex.unlock(&doneQMutex);
        }
    }
    return 0;
}

int main () {
    signal(SIGINT, terminate); // cleanup [Control + C]
    signal(SIGQUIT, terminate); // cleanup
    jobSemaphore = newSemaphore(0);
    doneSemaphore = newSemaphore(0);
    jobQMutex = newMutex();
    doneQMutex = newMutex();
    jobQueue = newQueue();
    doneQueue = newQueue();
    number = newSharedMemory(sizeof (i32)), clientflag = newSharedMemory(sizeof (i32));
    slots = newSharedMemory(sizeof (i32)*10), serverflag = newSharedMemory(sizeof (i32)*10);

    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (!pid) {
        number.update(&number), clientflag.update(&clientflag);
        slots.update(&slots), serverflag.update(&serverflag);
    // child process ("the server")
        int numThreads = 4;

        // for (int i = 0; i < numThreads; ++i)
        // {
        //     Thread t = newThread();
        //     t.startDetached(&t, worker, 0);
        // }

        jobQMutex.lock(&jobQMutex);
        number.data = (void*)8;
        printf("number %d\n", (i32)number.data);
        slots.data = (void*)82;
        void *p = slots.data + (sizeof (i32));
        p = (void*) 86;
        printf("slots %d\n", (i32)slots.data);
        printf("slots %d\n", (i32)(slots.data + (sizeof (i32))));

        // uint32_t x = 77;

        number.set(&number, 14, 0);
        printf("getnum: %d\n", number.get(&number, 0));

        slots.set(&slots, 6532, 1);
        printf("getnum: %d\n", slots.get(&slots, 1));

        // number.data = (void*)1009;

        // number.set(&number, 10, 0);
        printf("number %d\n", (i32)number.data);
        // slots.data = (void*)82;
        void *p2 = slots.data + (sizeof (i32))*6;
        p2 = (void*) 0;
        // printf("slots %d\n", (i32)slots.data);
        printf("slots %d\n", (i32)(slots.data + sizeof(i32)*6));
        // printf("slots %d\n", (int)slots.data+1);
        // perror("sdfaf");
        // number.print(&number);
        // number.getNumP(&number) = 88;
        // printf("%d\n", *(int*)number.data);

        // number.print(&number);
        Job job1; initJob(&job1, 1, 2);
        Job job2; initJob(&job2, 2, 30);
        jobQueue.push(&jobQueue, &job1);
        jobQueue.push(&jobQueue, &job2);
        printJob(&job1);
        printJob(&job2);
        jobSemaphore.signalX(&jobSemaphore, 2); // signal that there is a new job available
        jobQMutex.unlock(&jobQMutex);

    }
    // parent process ("the client")
    while(1) // wait for jobs to complete
    {
        Job j;
        int hasJobs = 0;
        doneQMutex.lock(&doneQMutex);
            hasJobs = !doneQueue.isEmpty(&doneQueue);
            if (hasJobs) {
                j = *(Job*) doneQueue.pop(&doneQueue);
            }
        doneQMutex.unlock(&doneQMutex);
        if (hasJobs)
        {
            // printf("slot #%d = %d\n", j.id, slots.getNumP(&slots)[j.id]);
            printJob(&j);
        }
    }
// printJob(&job1);
        // t.join(&t);


// clean up
    jobQueue.destroy(&jobQueue);
    doneQueue.destroy(&doneQueue);
    jobQMutex.free(&jobQMutex);
    doneQMutex.free(&doneQMutex);
    jobSemaphore.free(&jobSemaphore);


    // Queue q1 = newQueue();

    // q1.push(&q1, 8);
    // q1.push(&q1, 99);
    // q1.push(&q1, 89);
    // q1.print(&q1);
    // q1.printInt(&q1);

    // printf("length = %d\n", q1.length(&q1));
    // printf("isEmpty = %d\n", q1.isEmpty(&q1));


    // int x = (int) q1.pop(&q1);
    // printf("int x = %d\n", x);
    // printf("pop = %d\n", q1.pop(&q1));

    // q1.printInt(&q1);

    // q1.destroy(&q1);

    // q1.printInt(&q1);

    // q1.push(&q1, 100);
    // q1.printInt(&q1);


    // Thread t1 = newThread();
    // t1.startDetached(&t1, worker, 0);
    // t1.join(&t1);

}

// int main(int argc, char const *argv[])
// {
//     int numberShmid = newSharedMemory(sizeof(i32));
//     int slotsShmid = newSharedMemory(sizeof(i32) * 10);

//     int pid = fork();
//     if (pid == -1) {
//         perror("Error creating a new process/thread.");
//         return 1;
//     }
//     if (pid != 0) {
//         // parent process ("the client")
//         int bufferSize = sizeof(i32) * 100;
//         char userBuffer[bufferSize];
//         // char serverBuffer[bufferSize];

//         i32 *number = shmat(numberShmid, NULL, 0);
//         i32 *slot = shmat(slotsShmid, NULL, 0);

//         while (1) {

//             if (slot[1]) {// if server has something for us to read
//                 printf("parent received: %d\n", slot[1]);
//                 slot[1] = 0;
//             }

//             if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
//                 fgets(userBuffer, sizeof userBuffer, stdin);
//                 removeNewLine(userBuffer);
//                 if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
//                     puts("\nWaiting for threads to quit...");
//                     wait(NULL); // wait for child. TODO: actually tell child we are quitting
//                     if (shmctl(numberShmid, IPC_RMID, NULL) == -1 || shmctl(slotsShmid, IPC_RMID, NULL) == -1) {
//                         perror("shmctl");
//                     }
//                     puts("\n -- See you later!");
//                     break; // exit loop
//                 }

//                 *number = atoi(userBuffer); // send number to child
//                 printf("echo: %d\n", *number);
//             }

//         }
//         return 0;
//     }
//     // child process ("the server")
// // multithreaded
// // must handle up to 10 simultaneous requests without blocking
// //
// // take each input number (i32) and will start up either the number of specified threads if given
// // (see Req.17) or 32 threads.
// // Each thread will be responsible for factorising an integer derived from the input number that is rotated
// // right by a different number of bits. Given an input number input number is K, each thread
// // #X will factorise K rotated right by (K-1) bits. For example say K and it has N significant bits, then thread
// // #0 will factorise the number K rotated right by 0 bits, thread
// // #1 will factorise K rotated right by 1 bit, thread
// // #2 will factorise K rotated right by 2 bits etc.
// // Rotating an integer K by B bits = ( K >> B) | (K << 32 – B). CLARIFICATION: C= K << (32 – B); Rotated = ( K >> B) | C

//     i32 *number = shmat(numberShmid, NULL, 0);
//     i32 *slot = shmat(slotsShmid, NULL, 0);
//     pthread_t t1, t2, t3, t4;
//     // Thread t1 = newThread(), t2 = newThread(), t3 = newThread(), t4 = newThread();
//     int threadNum = 4;
//     int i = 0;
//     puts("start loop");
//     while (1) {
//         if (*number) {
//             printf("child received: %d\n", *number);
//             // insert multi threading here
//             // do a bitshift, qoue jobs
//             // threads take jobs and factorise
//             // i32 temp = *number;
//             printNum(*number);
//             // t1.start(&t1, *func, (void *)number);
//             pthread_create(&t1, NULL, *func, (void *)number);
//             i++;

//             *number = rotr32(*number, 1);
//             printNum(*number);
//             // t2.start(&t2, *func, (void *)number);
//             pthread_create(&t2, NULL, *func, (void *)number);
//             *number = rotr32(*number, 1);
//             printNum(*number);
//             // t3.start(&t3, *func, (void *)number);
//             pthread_create(&t3, NULL, *func, (void *)number);
//             *number = rotr32(*number, 1);
//             printNum(*number);
//             // t4.start(&t4, *func, (void *)number);
//             pthread_create(&t4, NULL, *func, (void *)number);



//             // t1.join(&t1);
//             // t1.join(&t2);
//             // t1.join(&t3);
//             // t1.join(&t4);

//             pthread_join(t1, NULL);
//             pthread_join(t2, NULL);
//             pthread_join(t3, NULL);
//             pthread_join(t4, NULL);

//             // slot[1] = factorise(*number);
//             // *number = 0;
//             for (int i = 0; i < threadNum; ++i)
//             {
//                 printf("%d\n", slot[i]);
//             }
//             exit(1);// for debugging only. comment if you like zombies
//         }
//     }
//     return 0;
// }
