/**
 * @file lib.c
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief library of utility functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
#include <stdint.h>  // for i32, to get 32bit-wide rotates, regardless of the size of int.
#include <limits.h>  // for CHAR_BIT
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#ifdef __MACH__ // macOS
#include <mach/clock.h>
#include <mach/mach.h>
#endif
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

int isdigitstr (char * buffer) {
    int r = 1;
    for (int i = 0; i < strlen(buffer); i++) {
        if (isdigit(buffer[i]) == 0) { // Non-zero value if the character is a numeric character, zero otherwise.
            r = 0;
        }
    }
    return r;
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

void removeNewLine(char* inStr) {
    if ((strlen(inStr) > 0) && (inStr[strlen(inStr) - 1] == '\n')) {
        inStr[strlen(inStr) - 1] = 0x00;
    }
}

// http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
void tsleep(float milliseconds)
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

struct timespec getTime() {
    struct timespec time;

    // https://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
    #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    time.tv_sec = mts.tv_sec;
    time.tv_nsec = mts.tv_nsec;
    #else
    clock_gettime(CLOCK_REALTIME, &time);
    #endif

    return time;
}

float getTimeLapsed(struct timespec start) {
    struct timespec end = getTime();
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
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
