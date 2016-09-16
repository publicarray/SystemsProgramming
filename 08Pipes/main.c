// Write a program that forks a child backend process.
// The parent the program operates as a user interface
// and the child does the real work of calculating
// the result of prefix mathematical expressions.
// The parent asks the user to enter expressions
// and then passes these to the child which calculates
// the result and returns it to the parent so that it can print it
// out on the screen. The parent and child use anonymous pipes to communicate.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "prefix.h"

int main(int argc, char const *argv[]) {

    int pipes[2];
    int pipes2[2];

    if (pipe(pipes) == -1 || pipe(pipes2) == -1) {
        perror("pipe()");
        exit(1);
    }

    int pid = fork();

    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return 1;
    }
    if (pid != 0) {
        // parent
        // printf("pid: %d\n", pid); // pid of the child
        while (1) {
            char buf[100];
            fgets(buf, 99, stdin);
            buf[strlen(buf)-1] = 0x00;
            write(pipes[1], buf, strlen(buf));

            if (strcmp(buf, "quit") == 0) {
                break;
            }

            int nr = read(pipes2[0], buf, 99);
            buf[nr] = 0x00;
            puts(buf);
        }

        puts("parent waiting for child to end");
        int status;
        wait(&status);
        printf("child exited status %d\n", WEXITSTATUS(status));
        exit(0);
    }

    while (1) {
        char buf[99];
        int nr = read(pipes[0], buf, 100);
        buf[nr] = 0x00;
        if (strcmp(buf, "quit") == 0) {
            break;
        }

        double val = prefixCalc(buf);
        sprintf(buf, "ans = %lf\n", val);
        write(pipes2[1], buf, strlen(buf));
    }
    return 0;
}
