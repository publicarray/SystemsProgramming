#ifndef SOCKETTYPE_H
#define SOCKETTYPE_H

#ifdef WIN32
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvAPi32.lib")
#include <winsock2.>
typedef SOCKET SocketType;
#else
typedef int SocketType;
#endif

typedef struct Socket_Struct {
    SocketType socket;
    void (*close)(struct Socket_Struct *self);
    int (*read)(struct Socket_Struct *self, char *buffer, int size);
    int (*write)(struct Socket_Struct *self, char *buffer, int size);
} Socket;

typedef struct Socket_Struct Client; // alias

typedef struct Server_Struct {
    int port;
    Socket socket;
    void (*setup)(struct Server_Struct *self);
    Socket (*accept)(struct Server_Struct *self);
    void (*close)(struct Server_Struct *self);
} Server;

Server newServer(int port);
Socket newSocket();
Client newClient(char *address, int port, int *error);
void initSocket();
void delSocket();
#endif
