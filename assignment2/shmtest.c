#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include "lib.h"

int shmid = 0;

void cleanup() {
    puts("\nCleaning up...!");
    if (shmid) {
        freeSharedMemory(shmid);
        shmid = 0;
    }
}
void terminate(int sig) {
    // clean up
    cleanup();
    puts("\nShut-down successfully!");
    exit(0);
}

int main () {
    i32* slots = NULL;
    signal(SIGINT, terminate); // cleanup [Control + C]
    signal(SIGQUIT, terminate); // cleanup
    shmid = newSharedMemory(sizeof(i32) * 10);
    printf("got shmid: %i\n", shmid);
    slots = getSharedMemory(shmid);
    printf("got p: %p\n", slots);
    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        wait(NULL); // wait for child to complete

        // parent process ("the client")
        printf("parent got: %u\n", slots[2]);
        cleanup();
        return 0;
    }
    // child
    slots[2] = 7;
    slots = getSharedMemory(shmid);
    printf("got p: %p\n", slots);

    printf("child got: %u\n", slots[2]);
}
