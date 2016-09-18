// Adapted from Luke's LSocketType
#include "SocketType.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else // linux
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
// #include <ifaddrs.h>
// #include <netinet/in.h>
// #include <netinet6/in6.h>
#endif

// List sockets/clients
// List id's
// int i
// int id
//
// int asyncRead(int totalSockets, );

// canRead() {
//    asyncRead() //select
// }
// canAccept(server, seconds2wait, miroseconds2wait)

// popen | piping
// _popen | piping

static void error_out (char *messgae) {
    printf("Error in SocketType.c: %s\n", messgae);
    exit(1);

}

int canRead(int fd, int seconds, int microseconds) {
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(fd, &readfd);
    struct timeval time = {seconds, microseconds};
    return select(fd+1, &readfd, NULL, NULL, &time) > 0;
}

static int Socket__canRead(Socket *self) {
    return canRead(self->socket, 0, 500000);
}

static void Server__close(Server *self) {
    self->socket.close(&self->socket);
}

static int Socket__read(Socket *self, char *buffer, int maxSize) {
    return recv(self->socket, buffer, maxSize, 0);
}

static int Socket__write(Socket *self, char *buffer, int maxSize) {
    return send(self->socket, buffer, maxSize, 0);
}

#ifdef WIN32
// windows
#else

void initSocket() {
    //
}

void delSocket() {
    //
}

static void __setup(Server *self) {
    struct sockaddr_in name;
    self->socket.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (self->socket.socket < 0) {
        error_out("Error when creating socket [__setup()]");
    }

    name.sin_family = AF_INET;
    name.sin_addr.s_addr = (INADDR_ANY);
    name.sin_port = htons(self->port);
    if (bind(self->socket.socket, (struct sockaddr *) &name, sizeof(name)) < 0) {
        error_out("Failed to bind to an interface, port may be in use [__setup()]");
    }

    int backlog = 50;
    listen(self->socket.socket, backlog);
}

Socket __accept(Server *self) {
    Socket ret = newSocket();
    struct sockaddr_in client_name;
    socklen_t clientLength = sizeof(client_name);
    ret.socket = accept(self->socket.socket, (struct sockaddr *) &client_name, &clientLength);
    return ret;
}

static void Socket__close(Socket *self) {
    close(self->socket);
}

Client newClient(char * address, int port, int * error) {
    Socket ret = newSocket();
    struct sockaddr_in serv_addr;
    struct hostent *server_name;

    ret.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ret.socket < 0) {
        error_out("Error opening socket [newClient()]");
    }
    server_name = gethostbyname(address);
    if (server_name == NULL) {
        error_out("Error opening socket, can't get the Server name. [newClient()]");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server_name->h_addr, server_name->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(ret.socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error creating connection to %s at port %i.\n", address, port);
        *error = 1;
    } else {
        *error = 0;
    }
    return ret;
}

#endif

// constructors
Server newServer(int port) {
    Server ret;
    ret.socket = newSocket(); // create socket
    ret.port = port; // set port number
    // set functions
    ret.setup = __setup;
    ret.accept = __accept;
    ret.close = Server__close;

    ret.setup(&ret);
    return ret;
}

Socket newSocket() {
    Socket ret;
     // set functions
    ret.close = Socket__close;
    ret.read = Socket__read;
    ret.write = Socket__write;
    ret.canRead = Socket__canRead;
    return ret;
}
