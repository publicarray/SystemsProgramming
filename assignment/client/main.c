#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../SocketType.h"
#include "../lib.h"

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
            fgets(buffer, sizeof buffer, stdin);
            client.write(&client, buffer, strlen(buffer));
            removeNewLine(buffer);
            if (strcmp(buffer, "quit") == 0) {
                break;
            }
        }

        int count = client.read(&client, buffer, sizeof(buffer));
        buffer[count] = 0x0;
        puts(buffer);
    }

    client.close(&client);
    delSocket();
    return error;
}
