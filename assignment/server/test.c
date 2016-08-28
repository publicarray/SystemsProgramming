#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
// #include <sys/wait.h>
// #include <sys/types.h>

#include "../SocketType.h"

// static Server server;

int newThread (Socket com) {

    int pid = fork();

    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return -1;
    }
    if (pid != 0) {
        // parent
        printf("pid: %d\n", pid); // pid of the child
        wait(NULL);
        return 0; // return to main
    }
    // child pid = 0;
    char buf[3000];
    int count = com.read(&com, buf, sizeof(buf));
    buf[count] = 0x0;
    puts(buf); // exit child
    char *msg = "Pong!";
    com.write(&com, msg, strlen(msg));
    com.close(&com);
    exit(0);
};

// void signal_callback_handler(int signum) {
//     printf("\nReceived SIGINT\n");
//     server.close(&server);
//     delSocket();
//     exit(0);
// }

int main(int argc, char const *argv[]) {

    initSocket();
    Server server = newServer(5000); // port 80 requires sudo

    // signal(SIGINT, signal_callback_handler); // Register signal and signal handler

    while (1) {
        Socket com = server.accept(&server);
        newThread(com);
    }

    server.close(&server);
    delSocket();
    return 0;
}
