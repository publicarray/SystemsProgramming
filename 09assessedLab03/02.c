#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "../08Pipes/prefix.h"

#define BUFFERLEN 200
#define SHARED_MEM_SIZE 1024 // 1k
int main(int argc, char const *argv[])
{
    int shmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    int pid = fork();
    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent process
        char *readptr = NULL, *writeptr = NULL;
        writeptr = readptr = shmat(shmid, NULL, 0);
        readptr += 512;
        memset(writeptr, 0x00, SHARED_MEM_SIZE);

        while(1) {
            char buffer[BUFFERLEN];
            fgets(buffer, BUFFERLEN-1, stdin);
            buffer[strlen(buffer)-1] = 0x00;

            writeptr++;
            strcpy(writeptr, buffer); // write
            writeptr[-1] = '*';
            if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
                shmdt(&shmid);
                break;
            }

            while(*readptr != '*'); // wait for child
            // now * is set in readptr, read some data
            readptr++;
            printf("%s\n", readptr);
            readptr[-1] = 0x00;

            // reset
            writeptr = readptr = shmat(shmid, NULL, 0);
            writeptr += 512;
        }
        exit(0);
    }

    // child process
    while (1) {
        char *readptr = NULL, *writeptr = NULL;
        writeptr = readptr = shmat(shmid, NULL, 0);
        writeptr += 512;
        char buffer[BUFFERLEN];

        while(*readptr != '*'); // wait to read data
        // now * is set in readptr, read some data
        readptr++;
        strcpy(buffer, readptr); // read
        readptr[-1] = 0x00;
        printf("pid:%d > %s\n", getpid(), buffer);
        if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
            exit(0);
        }

        double value = prefixCalc(buffer);
        sprintf(buffer, "pid:%d < %lf\n", getpid(), value);
        writeptr++;
        strcpy(writeptr, buffer); // write
        writeptr[-1] = '*';

        // reset
        writeptr = readptr = shmat(shmid, NULL, 0);
        writeptr += 512;
    }
    return 0;
}
