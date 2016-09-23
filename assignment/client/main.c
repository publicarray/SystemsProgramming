#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib.h"
#include "../../SocketType.h"
#include "../../01StringAndList/String.h"

int canRead(int fd, int seconds, int microseconds); // function declaration

int getCommand (char *request, int* saveOutput, int *fflag, char *filepath, String* filedata) {
    *fflag = 0;
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

    if ((argc == 1 || argc == 2) && strcmp(request, "put") == 0) { // if put command send the file
        // Reconstruct user input
        strConcatCS(filedata, request); // add the user input to the request
        if (*fflag) {
            strConcatCS(filedata, " -f"); // add the user input to the request
        }
        for (int i = 0; i < argc; i++) {
            strConcatC(filedata, ' '); // add the user input to the request
            strConcatCS(filedata, argv[i]); // add the user input to the request
        }
        strConcatC(filedata, '\n');
        // read file
        return readFile(argv[0], filedata);
    } else if (argc == 2) {
        strcpy(filepath, argv[1]);
        *saveOutput = 1;
    }

    *saveOutput = 0;
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

    initSocket();
    int error = 0, status = 0, count = 0, saveOutput = 0, forceSave = 0, bufferSize = 10000;
    struct timespec startTime;
    char filepath[600]; // TODO: allow long file names
    String filedata; strInit(&filedata);
    String buf; strInit(&buf); //
    char buffer[bufferSize];

    Client client = newClient(argv[0], portNumber, &error); // "127.0.0.1", 80
    if(!error) {
        while (1) { // send commands to server
            if (client.canRead(&client)) { // if the server send something
                // block to read the full message
                // while ((count = client.read(&client, buffer, sizeof(buffer)))) {
                //     printf("\ncount:%d\n",count);
                //     if (count == -1) {
                //         puts("error reading data from server");
                //     } else if (count == 0) {
                //         puts("server closed connection");
                //         exit(0);
                //     }
                //     buffer[count] = 0x00;
                //     strConcatCS(&buf, buffer);
                //     printServer(buf.data);
                //     // TODO: if end of input break loop / Need to know ahead of time what the file size is
                // }
                count = client.read(&client, buffer, sizeof(buffer));
                if (count == -1) {
                    puts("error reading data from server");
                } else if (count == 0) {
                    puts("server closed connection");
                    exit(0);
                }
                buffer[count] = 0x00;

                if (saveOutput) {
                    saveToFile(filepath, buffer, forceSave, NULL);
                } else {
                    printServer(buffer); // add colour to message - defined in lib.c
                }
                printf(BLU "Duration: %.20f second(s)" NRM "\n", getTimeLapsed(startTime));
            }
            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(buffer, sizeof buffer, stdin);
                // removeNewLine(buffer);
                status = getCommand(buffer, &saveOutput,  &forceSave, filepath, &filedata);
                if (filedata.length > 0 && status == 0) {
                    client.write(&client, filedata.data, filedata.length); // send user input and file data
                } else if (status == 0){
                    client.write(&client, buffer, strlen(buffer)); // send user input
                }
                strClean(&filedata); // reset buffer
                startTime = getTime();
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
