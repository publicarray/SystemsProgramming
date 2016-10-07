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
#include <pthread.h>
// #include <sys/types.h>
#include "lib.h"
// #include "../../01StringAndList/String.h"

#define BUFFERLEN 200
#define SHARED_MEM_SIZE 1024 // 1k

void* func(void* args) {
    int temp = rotr32( *(uint32_t*)args, 1);
    // int temp = (*(int *)args)++;
    // *(int32_t *)args += 1;
    *(uint32_t*) args = temp;
    printf("rotr32: %u\n", *(uint32_t *)args);
    return args;
}

int main(int argc, char const *argv[])
{

    int numberShmid = newSharedMemory(sizeof(uint32_t));
    int slotsShmid = newSharedMemory(sizeof(uint32_t) * 10);

    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent process ("the client")
        int bufferSize = sizeof(uint32_t) * 100;
        char userBuffer[bufferSize];
        // char serverBuffer[bufferSize];

        uint32_t *number = shmat(numberShmid, NULL, 0);
        uint32_t *slot = shmat(slotsShmid, NULL, 0);

        while (1) {

            if (slot[1]) {// if server has something for us to read
                printf("parent received: %d\n", slot[1]);
                slot[1] = 0;
            }

            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(userBuffer, sizeof userBuffer, stdin);
                    removeNewLine(userBuffer);
                if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
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
// take each input number (unsigned long) and will start up either the number of specified threads if given
// (see Req.17) or 32 threads.
// Each thread will be responsible for factorising an integer derived from the input number that is rotated
// right by a different number of bits. Given an input number input number is K, each thread
// #X will factorise K rotated right by (K-1) bits. For example say K and it has N significant bits, then thread
// #0 will factorise the number K rotated right by 0 bits, thread
// #1 will factorise K rotated right by 1 bit, thread
// #2 will factorise K rotated right by 2 bits etc.
// Rotating an integer K by B bits = ( K >> B) | (K << 32 – B). CLARIFICATION: C= K << (32 – B); Rotated = ( K >> B) | C

    uint32_t *number = shmat(numberShmid, NULL, 0);
    uint32_t *slot = shmat(slotsShmid, NULL, 0);
    pthread_t t1, t2, t3, t4;

    while (1) {
        if (*number) {
            printf("child received: %d\n", *number);
            // insert multi threading here
            pthread_create(&t1, NULL, *func, (void *)number);
            pthread_create(&t2, NULL, *func, (void *)number);
            pthread_create(&t3, NULL, *func, (void *)number);
            pthread_create(&t4, NULL, *func, (void *)number);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);
            pthread_join(t3, NULL);
            pthread_join(t4, NULL);

            slot[1] = factorise(*number);
            *number = 0;
            exit(1);// for debugging only. comment if you like zombies
        }
    }
    return 0;
}
