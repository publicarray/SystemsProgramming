#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../SocketType.h"

int newThread (Socket com) {

    int pid = fork();
    if (pid == -1) {
        puts("Error creating a new process/thread.");
        return -1;
    }
    if (pid != 0) {
        // parent
        wait(NULL);
        return 0;
    }
    // child
    char buf[3000];
    int count = com.read(&com, buf, sizeof(buf));
    buf[count] = 0x0;
    puts(buf);

    com.close(&com);
    exit(0);
};

int main(int argc, char const *argv[])
{
    initSocket();

    Server server = newServer(5000);
    while (1) {
        Socket com = server.accept(&server);
        newThread(com);
    }

    server.close(&server);
    delSocket();
    return 0;
}
