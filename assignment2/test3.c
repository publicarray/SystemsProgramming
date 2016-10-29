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
#include <sys/shm.h>
// #include <sys/types.h>
#include "Thread.h"
#include "SharedMemory.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Semaphore.h"
#include "Queue.h"
#include "lib.h"
// #include "../../01StringAandList/String.h"

#define BUFFERLEN 200
#define SHARED_MEM_SIZE 1024 // 1k

SharedMemory mem;
int numberShmid;
int slotsShmid;

i32 *number;
i32 *slot;

void* func(void* num) {
    // int temp = rotr32( *(i32*)args, 1);
    // int temp = (*(int *)args)++;
    // i32 temp =  *(i32*) args;
    // *(int32_t *)args += 1;
    // *(i32*) args = temp;
    mem.data32[2] = 7;
    slot[1] = 7;
    printf("thread got: %u\n", *(i32*) num);
    printf("thread32 got: %u\n", mem.data32[2]);
    return (void*) factorise(*(i32*) num);
    // return NULL;

}

void terminate (int sig) {
    // clean up
    if (shmctl(numberShmid, IPC_RMID, NULL) == -1 || shmctl(slotsShmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }
    puts("\nShut-down successfully!");
    exit(0);
}


int main(int argc, char const *argv[]) {
    signal(SIGINT, terminate); // cleanup [Control + C]
    signal(SIGQUIT, terminate); // cleanup

    mem = newSharedMemory(sizeof(i32) * 10);
    numberShmid = shmget(IPC_PRIVATE, sizeof(i32), IPC_CREAT | 0666);
    slotsShmid = shmget(IPC_PRIVATE, sizeof(i32) * 10, IPC_CREAT | 0666);
    if (numberShmid == -1 || slotsShmid == -1) {
        perror("shmget");
        exit(1);
    }


    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent process ("the client")
        int bufferSize = sizeof(i32) * 100;
        char userBuffer[bufferSize];
        // char serverBuffer[bufferSize];

        number = shmat(numberShmid, NULL, 0);
        slot = shmat(slotsShmid, NULL, 0);

        while (1) {

            if (slot[1]) {// if server has something for us to read
                printf("parent received: %d\n", slot[1]);
                slot[1] = 0;
            }

            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(userBuffer, sizeof userBuffer, stdin);
                removeNewLine(userBuffer);
                if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
                    puts("\nWaiting for threads to quit...");
                    wait(NULL); // wait for child. TODO: actually tell child we are quitting
                    if (shmctl(numberShmid, IPC_RMID, NULL) == -1 || shmctl(slotsShmid, IPC_RMID, NULL) == -1) {
                        perror("shmctl");
                    }
                    puts("\n -- See you later!");
                    break; // exit loop
                }

                *number = atoi(userBuffer); // send number to child
                printf("echo: %d\n", *number);
            }

        }
        return 0;
    }
    // child process ("the server")
// multithreaded
// must handle up to 10 simultaneous requests without blocking
//
// take each input number (i32) and will start up either the number of specified threads if given
// (see Req.17) or 32 threads.
// Each thread will be responsible for factorising an integer derived from the input number that is rotated
// right by a different number of bits. Given an input number input number is K, each thread
// #X will factorise K rotated right by (K-1) bits. For example say K and it has N significant bits, then thread
// #0 will factorise the number K rotated right by 0 bits, thread
// #1 will factorise K rotated right by 1 bit, thread
// #2 will factorise K rotated right by 2 bits etc.
// Rotating an integer K by B bits = ( K >> B) | (K << 32 – B). CLARIFICATION: C= K << (32 – B); Rotated = ( K >> B) | C

    number = shmat(numberShmid, NULL, 0);
    slot = shmat(slotsShmid, NULL, 0);
    mem.update(&mem);
    pthread_t t1, t2, t3, t4;
    // Thread t1 = newThread(), t2 = newThread(), t3 = newThread(), t4 = newThread();
    int threadNum = 4;
    int i = 0;
    puts("start loop");
    while (1) {
        if (*number) {
            printf("child received: %d\n", *number);
            // insert multi threading here
            // do a bitshift, qoue jobs
            // threads take jobs and factorise
            // i32 temp = *number;
            printNum(*number);
            // t1.start(&t1, *func, (void *)number);
            pthread_create(&t1, NULL, *func, (void *)number);
            i++;

            *number = rotr32(*number, 1);
            printNum(*number);
            // t2.start(&t2, *func, (void *)number);
            pthread_create(&t2, NULL, *func, (void *)number);
            *number = rotr32(*number, 1);
            printNum(*number);
            // t3.start(&t3, *func, (void *)number);
            pthread_create(&t3, NULL, *func, (void *)number);
            *number = rotr32(*number, 1);
            printNum(*number);
            // t4.start(&t4, *func, (void *)number);
            pthread_create(&t4, NULL, *func, (void *)number);



            // t1.join(&t1);
            // t1.join(&t2);
            // t1.join(&t3);
            // t1.join(&t4);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);
            pthread_join(t3, NULL);
            pthread_join(t4, NULL);

            // slot[1] = factorise(*number);
            // *number = 0;
            for (int i = 0; i < threadNum; ++i)
            {
                printf("%d\n", slot[i]);
            }

            // clean up
            if (shmctl(numberShmid, IPC_RMID, NULL) == -1 || shmctl(slotsShmid, IPC_RMID, NULL) == -1) {
                perror("shmctl");
            }

            exit(1);// for debugging only. comment if you like zombies
        }
    }
    return 0;
}
