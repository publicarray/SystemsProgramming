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
#include "../lib.h"
// #include "../../01StringAndList/String.h"

#define BUFFERLEN 200
#define SHARED_MEM_SIZE 1024 // 1k

int main(int argc, char const *argv[])
{
    // struct protocol
    // {
    //     uint32_t number;
    //     uint32_t slot[10];
    //     int clientflag;
    //     int serverflag[10];
    // };

    int numberShmid = newSharedMemory(sizeof(uint32_t));
    int slotsShmid = newSharedMemory(sizeof(uint32_t) * 10);

    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent process ("the client?")
        int bufferSize = sizeof(uint32_t) * 100;
        char userBuffer[bufferSize];
        // char serverBuffer[bufferSize];

        uint32_t *number = shmat(numberShmid, NULL, 0);
        uint32_t *slot = shmat(slotsShmid, NULL, 0);

        while (1) {
            // if (canRead(STDIN_FILENO, 0, 500000)) { // if server has something for us to read
            //     // print output ... ->
            // }

            if (slot[1]) {
                printf("parent received: %d\n", slot[1]);
                slot[1] = 0;
            }

            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(userBuffer, sizeof userBuffer, stdin);
                    removeNewLine(userBuffer);
                if (strcmp(userBuffer, "q") == 0 || strcmp(userBuffer, "quit") == 0 || strcmp(userBuffer, "exit") == 0) {
                    wait(NULL); // wait for child. TODO: actually tell child we are quitting
                    puts("\n -- See you later!");
                    break; // exit loop
                }

                *number = atoi(userBuffer); // send number to child
                printf("echo: %d\n", *number);
            }

        }
        return 0;
    }
    // child process ("the server?")
    uint32_t *number = shmat(numberShmid, NULL, 0);
    uint32_t *slot = shmat(slotsShmid, NULL, 0);

    while (1) {
        if (*number) {
            printf("child received: %d\n", *number);
            *number = 0;
            slot[1] = factorise(*number);
        }
    }
    return 0;
}
