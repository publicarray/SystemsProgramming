//
//  main.c
//  server
//
//  Created by Sebastian Schmidt on 9/09/2016.
//  Copyright © 2016 Sebastian Schmidt. All rights reserved.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
// #include <sys/types.h>
#include "../../SocketType.h"
#include "../lib.h"
#include "../../01StringAndList/String.h"
#include "common.h"


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

// needed for put command
    int reqLen = strlen(request);
    char requestCopy [reqLen];
    strcpy(requestCopy, request);
    printf("requestCopy:%s\n", requestCopy);

    // Remove trailing newline
    removeNewLine(request);
    argCount = splitStr(request, arguments, maxArguments);

    if (arguments[0] && strcmp(arguments[0], "list") == 0) {
        list(argCount, arguments, response);
    } else if (arguments[0] && strcmp(arguments[0], "get") == 0) {
        get(argCount, arguments, response);
    } else if (arguments[0] && strcmp(arguments[0], "put") == 0) {
        strConcatCS(response, "received pudding\n");
        put(argCount, arguments, requestCopy, response);
        // char* filedData = NULL;
        // if ((filedData = nextLine(requestCopy)) == NULL) {
        //     strConcatCS(response, "Data not found!");
        //     return;
        // }
        // int fflag = 0;
        // saveToFile("test.txt", filedData, fflag, NULL);
        // strConcatCS(response, nextLine(requestCopy));
        // strConcatC(response, '\n');
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

int newProcess (Socket com) {
    int pid = fork();

    if (pid == -1) {
        perror("Error creating a new process/thread.");
        return -1;
    }
    if (pid != 0) {
        // parent:
        printf("pid: %d\n", pid); // pid of the child
        return pid; // return pid to main()
    }
    // child:
    int cpid =  getpid();
    char buffer[10000];
    String response; strInit(&response);
    int readCount, numProcesses = 0;
    while(1) {
        readCount = com.read(&com, buffer, sizeof(buffer));
        buffer[readCount] = 0x00;
        removeNewLine(buffer); // defined in lib.c
        // strToLower(buffer);
        if (strlen(buffer) == 0) {
            continue; // skip loop if empty string e.g. new line feeds
        }

        printf("%d: %s\n", cpid, buffer); // log request to server console

        if (readCount == 0 || strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) { // when readCount = 0 the peer closed it's half of the connection
            for (int i = 0; i < numProcesses; i++) { // wait for child processed before exiting
                wait(NULL); // blocking
            }
            break;
        } else if (readCount == -1) {
            perror("Error reading input from client");
            com.write(&com, "Sorry, I don't understand your request!\n", 41);
            break;
        }

        // create a new process for each command
        int pid = fork();

        if (pid == -1) {
            perror("Error creating a new process/thread.");
            com.write(&com, "Error: Unable to create new processes!\n", 40);
        }
        if (pid != 0) {
            // parent
            numProcesses++;
            int mypid = getpid();
            printf("pid: %d -> pid: %d\n", mypid, pid); // pid of the child
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
    exit(0); // exit child
};


int main(int argc, char *argv[]) {
    signal(SIGCHLD, childProcessExit); // listen when a child exits
    signal(SIGINT, terminate); // cleanup [Control + C]
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
        if (newProcess(com) == -1) {
            com.write(&com, "Error: Unable to create new processes!\n", 40);
        }
    }

    server.close(&server);
    delSocket();
    return 0;
}
