#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <unistd.h>
#include "../lib.h"

int main()
{

    char buf [5000];
    int server = makeServerSocket(5000);
    // SSocket com = server.accept(&server);

    if (server < 0) {
        exit(-1);
    }

    while (1) {
        int fd = accept(server, NULL, NULL);
        if (fd < 0) {
            puts("null");
            break;
        }
        int count = read(fd, buf, sizeof(buf));
        buf[count] = 0x0;
        puts(buf);
        close(fd);
    }
    return 0;
}
