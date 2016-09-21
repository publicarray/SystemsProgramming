//
//  main.c
//  server
//
//  Created by Sebastian Schmidt on 9/09/2016.
//  Copyright © 2016 Sebastian Schmidt. All rights reserved.
//
//
// create a new connection for each command, for every command fork the process

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h> // errno
// #include <sys/wait.h>
#include <sys/time.h>
// #include <sys/types.h>
#include "../../SocketType.h"
#include "../lib.h"
#include "../../01StringAndList/String.h"
#include "common.h"
// #include "../../01StringAndList/List.h"

// cc -g main.c ../../SocketType.c ../lib.c ../../01StringAndList/String.c ls.c cat.c -o server && ./server -p 8080


// http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
void childProcessExit (int sig) {
    int pid;
    while ((pid = waitpid(-1, 0, WNOHANG)) > 0) { // non blocking
        printf("Child Process Exited: %d!\n", pid);
        if (pid == -1 && errno != 10) { // 10 = No child processes
            perror("childProcessExit waitpid()");
        }
    }
}

void terminate (int sig) {
    int pid;
    while ((pid = wait(NULL)) > 0) { // wait for processes to finish
        printf("Stopping Server: %d!\n", pid);
        if (pid == -1 && errno != 10) { // 10 = No child processes
            perror("terminate wait()");
        }
    }

    exit(0);
}

void console(char *request, String *response) {
    int argCount, maxArguments = 10;
    char *arguments[maxArguments];
    // Remove trailing newline
    removeNewLine(request);
    argCount = splitStr(request, arguments, maxArguments);

//debug
    // for (int i = 0;i < argCount; i++){
    //     printf("%d: %s\n", i, arguments[i]);
    // }

    if (arguments[0] && strcmp(arguments[0], "list") == 0) {
        list(argCount, arguments, response);
    } else if (arguments[0] && strcmp(arguments[0], "get") == 0) {
        // get(argCount, arguments, response);
        get(arguments[1], response);
    } else if (arguments[0] && strcmp(arguments[0], "put") == 0) {
        strConcatCS(response, "Command put found\n");
        // put(arguments[1], response);
    } else if (arguments[0] && strcmp(arguments[0], "sys") == 0) {
        sys(response);
    } else if (arguments[0] && strcmp(arguments[0], "delay") == 0) {
        if (argCount != 2 || atoi(arguments[1]) == 0) {
            strConcatCS(response, "Usage: delay [time in seconds]\n");
        } else {
            delay(atoi(arguments[1]), response);
        }
    } else {
        strConcatCS(response, "Command not found\n");
    }
}

int newThread (Socket com) {
    int pid = fork();

    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return -1;
    }
    if (pid != 0) {
        // parent
        printf("pid: %d\n", pid); // pid of the child
        return pid; // return pid to main
    }

    // child pid = 0;
    int cpid =  getpid();
    char buffer[3000];
    // char answer[3000];
    String response; strInit(&response);
    int count;
    int numProcesses = 0;
    while(1) {
        count = com.read(&com, buffer, sizeof(buffer));
        buffer[count] = 0x0;
        removeNewLine(buffer);
        // strToLower(buffer);
        printf("%d: %s\n", cpid, buffer); // log request

        if (count == 0 || strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) { // when count = 0 the peer closed it's half of the connection
            for (int i = 0; i < numProcesses; i++) { // wait for child processed before exiting
                if ((pid = wait(NULL)) > 0) { // blocking
                    printf("Child's child Process Exited: %d!\n", pid);
                } else if (pid == -1 && errno != 10) { // 10 = No child processes
                    perror("Child waitpid()");
                }
            }
            break;
        } else if (count == -1) {
            puts("Error reading input from client");
            break;
        }

        strClean(&response); // empty string

        int pid = fork(); // create a new process for each command

        if (pid == -1) {
            puts("Error creating a new process/thread.");
            return -1;
        }
        if (pid != 0) {
            // parent
            numProcesses++;
            printf("pid: %d\n", pid); // pid of the child
        } else {
            console(buffer, &response); // run command
            if (response.length > 0) {
                com.write(&com, response.data, response.length); // send response
            }
            exit(0); // exit child
        }
    }

    com.close(&com);
    strFree(&response);
    exit(0);// exit child
};


int main(int argc, char *argv[]) {
    signal(SIGCHLD, childProcessExit); // listen when a child exits
    signal(SIGINT, terminate); // cleanup
    signal(SIGQUIT, terminate); // cleanup

    opterr = 0; // disable getopt's own error messages e.g. case '?'
    int c, portNumber = 80; // port 80 requires sudo

    while ((c = getopt(argc, argv, "p:")) != EOF) {
        switch (c) {
            case 'p':
                portNumber = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "invalid option: -%c\n", optopt);
        }
    }

    // skip arguments that have been processed
    argv += optind;
    argc -= optind;

    if (argc != 0) {
        printf("usage: server [-p]\nARGUMENTS:\n    -p Port\n \n");
        exit(1);
    }

    if (portNumber < 1024 && getuid()) { // if getuid() You are not root!
        puts("Error: To bind port numbers under 1024 requires root.");
        return 1;
    }

//
    initSocket();
    Server server = newServer(portNumber);

    while (1) {
        Socket com = server.accept(&server);
        newThread(com);
    }
    // newCMD(repom); // new process for each command

    server.close(&server);
    delSocket();
    return 0;
}
