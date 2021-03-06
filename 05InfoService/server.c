#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
// #include <sys/wait.h>
// #include <sys/types.h>
#include "../SocketType.h"
#include "../assignment/lib.h"
#include "../01StringAndList/String.h"
// #include "../../01StringAndList/List.h"

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
    if (arguments[0] && strcmp(arguments[0], "time") == 0) {
        localTime(response);
    }

    if (arguments[0] && strcmp(arguments[0], "list") == 0) {
         if (argCount == 0) {
            // listdir(".", response); // if no arguments use current directory
        } else if (argCount > 0) {
            // listdir(arguments[optind], response);
        }
    }

    // strConcatCS(response, request);
    // strAddChar(response, '\n');
}

int newThread (Socket com) {

    int pid = fork();

    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return -1;
    }
    if (pid != 0) {
        // parent
        // printf("pid: %d\n", pid); // pid of the child
        wait(NULL);
        return 0; // return to main
    }
    // child pid = 0;

    char buffer[3000];
    // char answer[3000];
    String response; strInit(&response);
    int count;

    while(1) {
        count = com.read(&com, buffer, sizeof(buffer));
        buffer[count] = 0x0;
        removeNewLine(buffer);
        // strToLower(buffer);
        puts(buffer); // log request

        if (count == 0 || strcmp(buffer, "quit") == 0) { // when count = 0 the peer closed it's half of the connection
            break;
        } else if (count == -1) {
            puts("Error reading input from client");
            break;
        }

        console(buffer, &response);
        com.write(&com, response.data, response.length);
    }

    com.close(&com);
    strFree(&response);
    exit(0);// exit child
};


int main(int argc, char *argv[]) {

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

    server.close(&server);
    delSocket();
    return 0;
}
