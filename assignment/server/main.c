#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
// #include <sys/wait.h>
// #include <sys/types.h>
#include "../SocketType.h"
#include "../lib.h"
#include "../../01StringAndList/String.h"
// #include "../../01StringAndList/List.h"

void console(char *request, String *response) {
    strConcatCS(response, request);
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
    }

    com.write(&com, response.data, response.length);
    com.close(&com);
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
