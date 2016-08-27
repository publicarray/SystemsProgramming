#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <unistd.h>
// #include <../lib.h>

int main()
{

    int sock = makeServerSocket(80, 5);

    if (sock < 0) {
        exit(-1);
    }

    while (1) {
        int fd = accept(sock, NULL, NULL);
        if (fd < 0) {
            break;
        }
        // process_request(fd);
        close(fd);
    }
    return 0;
}
