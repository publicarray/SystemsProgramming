#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "../08Pipes/prefix.h"

#define BUFFERLEN 200
int main(int argc, char const *argv[])
{
    int parentPipes[2];
    int childPipes[2];

    if (pipe(parentPipes) == -1 || pipe(childPipes) == -1) {
        perror("pipe()");
        exit(1);
    }

    int pid = fork();

    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent process
        // printf("child pid: %d\n", pid); // pid of the child
        while(1) {
            char buffer[BUFFERLEN];
            fgets(buffer, BUFFERLEN-1, stdin);
            buffer[strlen(buffer)-1] = 0x00;
            int numWritten = write(parentPipes[1], buffer, strlen(buffer));
            if (numWritten == -1) {
                 perror("write(parentPipes[1])");
            }
            if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
                break;
            }
            int readCount = read(childPipes[0], buffer, BUFFERLEN-1);
            if (readCount == -1) {
                perror("read(childPipes[0])");
            }
            buffer[readCount] = 0x00;
            printf("%s", buffer);
        }
        exit(0);
    }

    // child process
     while (1) {
        char buffer[BUFFERLEN];
        int readCount = read(parentPipes[0], buffer, 100);
        if (readCount == -1) {
            perror("read(parentPipes[0])");
        }
        buffer[readCount] = 0x00;
        printf("pid:%d > %s\n", getpid(), buffer);
        if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
            break;
        }

        double value = prefixCalc(buffer);
        sprintf(buffer, "pid:%d < %lf\n", getpid(), value);
        int numWritten = write(childPipes[1], buffer, strlen(buffer));
        if (numWritten == -1) {
            perror("write(childPipes[1])");
        }
    }

    return 0;
}
