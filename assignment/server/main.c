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
// #include <sys/types.h>
#include "../../SocketType.h"
#include "../lib.h"
#include "../../01StringAndList/String.h"
// #include "../../01StringAndList/List.h"

// cc -g main.c ../../SocketType.c ../lib.c ../../01StringAndList/String.c ls.c cat.c -o server && ./server -p 8080


// http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
void childProcessExit (int sig) {
    int pid;
    while ((pid = waitpid(-1, 0, WNOHANG)) > 0) { // non blocking
        printf("killed: %d!\n", pid);
        // if (kill(pid, SIGTERM) == -1) { // kill(): No such process
        //     perror("kill()");
        // }
    }

    if (pid == -1) {
        if (errno != 10) { // 10 = No child processes
            perror("waitpid()");
        }
    }
}

void terminate (int sig) {
    int pid;
    while ((pid = wait(NULL)) > 0) { // wait for processes to finish
        printf("killed: %d!\n", pid);
        // if (kill(pid, SIGTERM) == -1) { // kill(): No such process
        //     perror("kill()");
        // }
    }

    if (pid == -1) {
        if (errno != 10) { // 10 = No child processes
            perror("wait()");
        }
    }

    exit(0);
}

void console(char *request, String *response) {
    // reset getopt
    optreset = 1;
    optind = 1;


    // opterr = 0; // disable getopt's own error messages e.g. case '?'
    int c, fflag = 0, lflag = 0, argCount = 0, maxArguments = 10;
    // char inStr[3000];
    char *arguments[maxArguments];
    // char tempbuf[200];
    // Remove trailing newline
    removeNewLine(request);
    argCount = splitStr(request, arguments, maxArguments);
    // printf("argCount: %i\n", argCount);
    while ((c = getopt(argCount, arguments, "fl")) != EOF) {
        switch (c) {
            case 'f':
                fflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case '?':
                response->length += sprintf(response->data+response->length, "invalid option: -%c\n", optopt);
                // sprintf(tempbuf, "invalid option: -%c\n", optopt);
                // strConcatCS(response, tempbuf);
        }
    }

    if (fflag) printf("Option f is set\n");
    if (lflag) printf("Option l is set\n");

    argCount -= optind;
    // printf("argCount (after): %i\n", argCount);
    if (argCount > 0 && strcmp(arguments[0], "dump") == 0) {
        dump(arguments[1]);
    }

    if (arguments[0] && strcmp(arguments[0], "list") == 0) {
         if (argCount == 0) {
            listdir(".", response); // if no arguments use current directory
        } else if (argCount > 0) {
            listdir(arguments[optind], response);
        }
    }

    // strConcatCS(response, request);
    strAddChar(response, '\n');
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

    while(1) {
        count = com.read(&com, buffer, sizeof(buffer));
        buffer[count] = 0x0;
        removeNewLine(buffer);
        // strToLower(buffer);
        printf("%d: %s\n",cpid, buffer); // log request

        if (count == 0 || strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) { // when count = 0 the peer closed it's half of the connection
            break;
        } else if (count == -1) {
            puts("Error reading input from client");
            break;
        }

        console(buffer, &response);
    }

    com.write(&com, response.data, response.length);
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
