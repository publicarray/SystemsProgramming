/**
 * @file lib.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief library of utility functions.
 */

#ifndef LIB_H
#define LIB_H

// Colours for the terminal.
#define NRM  "\x1B[0;0m" /**< Reset to normal */
#define RED  "\x1B[31m" /**< Red */
#define GRN  "\x1B[32m" /**< Green */
#define BOLD "\x1B[1;" /**< Bold */
#define YEL  "\x1B[33m" /**< Yellow */
#define BLU  "\x1B[34m" /**< Blue */
#define MAG  "\x1B[35m" /**< Magenta */
#define CYN  "\x1B[36m" /**< Cyan */
#define WHT  "\x1B[37m" /**< White */

#define i32 uint32_t /**< unsigned int */
#include <stdint.h>

/**
 * Bit-shift to the left
 * @param  value The number to shift
 * @param  count The number of times to shift the value
 * @return       The shifted output
 * @see https://en.wikipedia.org/wiki/Circular_shift
 */
i32 rotl32 (i32 value, unsigned int count);

/**
 * Bit-shift to the right
 * @param  value The number to shift
 * @param  count The number of times to shift the value
 * @return       The shifted output
 * @see https://en.wikipedia.org/wiki/Circular_shift
 */
i32 rotr32 (i32 value, unsigned int count);

/**
 * Calculate the first factor using Trial Division
 * @param  n The number to factorise
 * @return   The first factor
 * @see https://en.wikipedia.org/wiki/Trial_division
 *
 */
i32 factorise(i32 n);

/**
 * Used to detect if there is data to read from, usually from STDIN_FILENO.
 * @param  fd           File descriptor to read from (readfds)
 * @param  seconds      Seconds to wait for input/data (timeout)
 * @param  microseconds Micro-seconds to wait for input/data (timeout)
 * @return              On success, return the number of file descriptors (1)
 * which may be zero if the timeout expires before anything interesting happens.
 * On error, -1 is returned, and errno is set appropriately; the sets and
 * timeout become undefined, so do not rely on their contents after an error.
 * @see https://linux.die.net/man/2/select
 */
int canRead(int fd, int seconds, int microseconds);

/**
 * Creates new shared memory
 * @param  size
 * @return shmid Shared Memory Id, used for all other shared memory functions.
 */
int newSharedMem(unsigned int size);

/**
 * Gives the memory segment back to the Operating System
 * @param shmid Shared Memory Id
 */
void freeSharedMem(int shmid);

/**
 * Attach the process to the shared memory segment.
 * @param  shmid Shared Memory Id
 * @return       Pointer to the shared memory
 */
void * getSharedMem(int shmid);

/**
 * Removes extra newline character from string, generally used when input is from user typed standard input.
 * @param inStr cleaned string
 */
void removeNewLine(char* inStr);
/**
 * Prints the 32 bit number (unsigned int) as binary to standard output.
 * @param num Number to print
 */
void printNum(i32 num);

/**
 * Test if the string is a digit
 * @param  buffer String to check against
 * @return        1 if true (is a digit), 0 if false
 * @see http://www.cplusplus.com/reference/cctype/isdigit/
 */
int isdigitstr (char * buffer);

/**
 * Sleep current thread by the specified time
 * @param milliseconds max time the thread sleeps
 * @see https://linux.die.net/man/2/nanosleep
 * @see http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
 */
void tsleep(float milliseconds);

/**
 * Get current time from system clock and store it in a struct
 * @return Current Time
 * @see https://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
 * @see https://linux.die.net/man/3/clock_gettime
 */
struct timespec getTime();

/**
 * Get the duration from start to now in seconds
 * @param  start Usually from getTime()
 * @return       Duration in seconds
 */
float getTimeLapsed(struct timespec start);

#endif
