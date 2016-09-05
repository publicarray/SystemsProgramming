#include <stdio.h>
#include <string.h>
#include "../../SocketType.h"

int main(int argc, char const *argv[])
{
    initSocket();
    int error;

    Client c = newClient("127.0.0.1", 5000, &error);

    if(!error) {
        char buf[3000];
        char *msg = "Ping!";
        c.write(&c, msg, strlen(msg));
        int count = c.read(&c, buf, sizeof(buf));
        buf[count] = 0x0;
        puts(buf);
    }
    c.close(&c);
    delSocket();
    return 0;
}
