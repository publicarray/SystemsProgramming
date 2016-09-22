#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../SocketType.h"
#include "../lib.h"

int canRead(int fd, int seconds, int microseconds); // function declaration

// void print(char *message) {
//     printf(CYN "%s" NRM, message);
// }

int getSaveCommand (char *request, int *fflag, char *filepath) {
    int argc, maxArguments = 10;
    char *argv[maxArguments];

    // Remove trailing newline
    removeNewLine(request);
    argc = splitStr(request, argv, maxArguments);

    // reset getopt
    optreset = 1;
    optind = 1;

    int c;
    while ((c = getopt(argc, argv, "f")) != EOF) {
        switch (c) {
            case 'f':
                *fflag = 1;
                break;
        }
    }

    // argv += optind;
    for (int i = 0; i < (argc-optind); i++) {
        argv[i] = argv[i+optind];
    }
    argc -= optind;

    if (argc == 2) {
        strcpy(filepath, argv[1]);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    opterr = 0; // disable getopt's own error messages e.g. case '?'
    int c, portNumber = 80;

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

    if (argc != 1) {
        printf("usage: client [-p] Hostname or IP Address\nARGUMENTS:\n    -p Port\n");
        exit(1);
    }

//
    initSocket();
    int error = 0;
    struct timespec startTime;
    int saveOutput = 0, forceSave = 0;
    char filepath[600]; // todo
    Client client = newClient(argv[0], portNumber, &error); // "127.0.0.1", 80
    if(!error) {
        char buffer[3000];
        while (1) { // send commands to server
            if (client.canRead(&client)) { // if the server send something
                int count = client.read(&client, buffer, sizeof(buffer));
                buffer[count] = 0x00;
                if (saveOutput) {
                    saveToFile(filepath, buffer, forceSave);
                } else {
                    printf(CYN "%s" NRM, buffer);
                }
                printf(BLU "Duration: %.20f second(s)" NRM "\n", getTimeLapsed(startTime));
            }
            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(buffer, sizeof buffer, stdin);
                // removeNewLine(buffer);
                client.write(&client, buffer, strlen(buffer));
                startTime = getTime();
                saveOutput = getSaveCommand(buffer, &forceSave, filepath);
            }

            if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "exit") == 0) {
                break;
            }
        }
    }
    client.close(&client);
    delSocket();
    return error;
}
