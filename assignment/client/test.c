#include <string.h>
#include "../SocketType.h"

int main(int argc, char const *argv[])
{
    initSocket();
    int error;

    Client c = newClient("127.0.0.1", 5000, &error);

    if(!error){
        char *msg = "Ping!";
        c.write(&c, msg, strlen(msg));
    }
    c.close(&c);
    delSocket();
    return 0;
}
