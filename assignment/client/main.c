#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../SocketType.h"
#include "../lib.h"

// cc -O3 main.c ../../SocketType.c ../lib.c -o client && ./client -p 8080 localhost
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
    int error;

    Client client = newClient(argv[0], portNumber, &error); // "127.0.0.1", 80

    if(!error) {
        char buffer[3000];
        while(1) { // send commands to server
            if (client.canRead(&client)) { // if the server send something
                int count = client.read(&client, buffer, sizeof(buffer));
                buffer[count] = 0x00;
                printf("%s", buffer);
            }
            if (canRead(STDIN_FILENO, 0, 500000)) { // if user typed something
                fgets(buffer, sizeof buffer, stdin);
                client.write(&client, buffer, strlen(buffer));
                removeNewLine(buffer);
            } else {
                // printf("%sWaiting for input%s\n", BLU, NRM);
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
