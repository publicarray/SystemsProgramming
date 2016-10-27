#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

// typedef uint_least32_t i32;

// start with how to bit shift in a cycle, threads and then inter-process/thread communications
// after that you need to look into how to build the state bard

// https://en.wikipedia.org/wiki/Circular_shift
/*
 * Shift operations in C are only defined for shift values which are
 * not negative and smaller than sizeof(value) * CHAR_BIT.
 * The mask, used with bitwise-and (&), prevents undefined behaviour
 * when the shift count is 0 or >= the width of unsigned int.
 */

#include <stdint.h>  // for i32, to get 32bit-wide rotates, regardless of the size of int.
#include <limits.h>  // for CHAR_BIT

i32 rotl32 (i32 value, unsigned int count) {
    i32 mask = (CHAR_BIT*sizeof(value)-1); // 32
    count &= mask;
    return (value<<count) | (value>>( (-count) & mask ));
}

i32 rotr32 (i32 value, unsigned int count) {
    i32 mask = (CHAR_BIT*sizeof(value)-1); // 32
    count &= mask;
    return (value>>count) | (value<<( (-count) & mask ));
}

int getBit(i32 num, int index)
{
    return (num & 0x80000000 >> index) != 0;
}

void printNum(i32 num)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        printf("%i", getBit(num, i)); //print out each bit
        if ((i+1) % 4 == 0 && i > 0 && i < 31) printf("-");
    }
    printf("\n");

}

// Trial Division
// http://www.connellybarnes.com/documents/factoring.pdf
// https://en.wikipedia.org/wiki/Trial_division

// 30 = 2,3,5 // prime factorisation
i32 factorise(i32 n) {
    for (i32 i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            // i = smallest factor found
            return i; // n/i = result
        }
    }

    return n; // found a prime number
}

// Fermat primality test
// a^(p-1) mod p = 1?
// a= a random number that is less than p
// p= int to check e.g 181
// 68^180 mod 181 = 1 so probably a prime with low error

#include <sys/types.h>
#include <sys/select.h>

int canRead(int fd, int seconds, int microseconds) {
    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(fd, &readfd);
    struct timeval time = {seconds, microseconds};
    return select(fd+1, &readfd, NULL, NULL, &time) > 0;
}

#include <sys/shm.h>

int newSharedMem(unsigned int size) {
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shmid < 0) {
        fprintf(stderr, "failed to create shared memory segment\n");
        perror("shmget");
        exit(-1);
    }
    return shmid;
}

void freeSharedMem(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }
}

void* getSharedMem(int shmid) {
    void* p = shmat(shmid, NULL, SHM_RND);
    if ((int)p == -1) {
        fprintf(stderr, "failed to attach\n");
        perror("shmat");
        exit(-1);
    }
    return p;
}

#include <string.h>

void removeNewLine(char* inStr) {
    if ((strlen(inStr) > 0) && (inStr[strlen(inStr) - 1] == '\n')) {
        inStr[strlen(inStr) - 1] = 0x00;
    }
}

#include <time.h>
// http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
void tsleep(unsigned int milliseconds)
{
    struct timespec time;
    time.tv_sec = (milliseconds / 1000);
    time.tv_nsec = (milliseconds - (time.tv_sec * 1000)) * 1000000L;

    while (nanosleep(&time, &time) == -1) {
        perror("nanosleep");
        continue;
    }
    return;
}

//
// int main(int argc, char const *argv[])
// {
//     printf("%d\n", rotl32(32, 3));
//     printf("%d\n", factorise(30));
//     printf("%d\n", factorise(54734711)); //prime:  54734711
//     printf("%d\n", factorise(-10));
//     printf("%d\n", factorise(-54734711));
//     printf("%d\n", rotl32(32, 32-1));
//     return 0;
// }
