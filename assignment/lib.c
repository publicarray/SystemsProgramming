#include <stdio.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
// #include <sys/types.h>
#import <netinet/in.h>
// #import <netinet6/in6.h>
// #import <ifaddrs.h>

int makeServerSocket(int portnum)
{
    int HOSTLEN = 400;

    // struct in_addr addr;
    // addr.s_addr = INADDR_ANY;

    struct sockaddr_in saddr;                       //‐ build our address here
    struct hostent *hp;                             //‐ this is part of our
    char hostname [HOSTLEN];                        //‐ address

    int sock_id = socket(AF_INET, SOCK_STREAM, 0);  //‐ get a socket
    if (sock_id == -1 ) {
        puts("unable to create a socket");
        return -1;
    }
    //‐‐ build address and bind it to socket ‐‐
    bzero((void*)&saddr, sizeof(saddr));

    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);
    bcopy ((void*) hp->h_addr, (void*) &saddr.sin_addr, hp->h_length);

    saddr.sin_family = AF_INET;             //‐ fill in addr famild
    saddr.sin_port = htons(portnum);        //‐ fill in socket port
    // saddr.sin_addr = htonl(INADDR_ANY);
    if (bind(sock_id, (struct sockaddr * )&saddr, sizeof(saddr)) != 0) {
        puts("unable to bind to an interface");
        return -1;
    }
    if (listen(sock_id, 5) != 0) {  //‐‐ wait for incoming calls return sock_id;
        puts("unable listen to the socket");
        return -1;
    }
    return sock_id;
}

int strLength(char *string)
{
    int length = 0;

    while (string[length] != '\0') {
        length++;
    }
    return length;
}

int splitStr(char* srcString, char* tokens[], int maxTokens)
{
    int numFound = 0;
    int saved = 0;
    int length = strLength(srcString);

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Maximum number of tokens reached\n");
            return numFound;
        }

        if (srcString[i] == ' ') {                      // if delimiter is found
            srcString[i] = 0x00;                    // replace with null char
            saved = 0;                              // marked as not saved (need to save on next iteration)
        } else if (saved == 0) {                        // if not saved
            tokens[numFound] = &srcString[i];       // save the address
            numFound++;
            saved = 1;                              // ignore (don't save anything)
        } else {                                        // all other characters
            saved = 1;                              // ignore (don't save anything)
        }
    }
    return numFound;
}
