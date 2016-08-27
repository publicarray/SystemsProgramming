#include <stdio.h>
#include "../SocketType.h"

int main(int argc, char const *argv[])
{
    initSocket();

    Server server = newServer(5000);
    Socket com = server.accept(&server);

    char buf[3000];
    int count = com.read(&com, buf, sizeof(buf));
    buf[count] = 0x0;
    puts(buf);

    com.close(&com);
    server.close(&server);
    delSocket();
    return 0;
}
