#ifndef LIB_H
#define LIB_H

#define NRM  "\x1B[0;0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define BOLD "\x1B[1;"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

#define i32 uint32_t
// #define i32 unsigned int

// #include "../01StringAndList/String.h"
#include <stdint.h>
// typedef uint_least32_t i32;


i32 rotl32 (i32 value, unsigned int count);
i32 rotr32 (i32 value, unsigned int count);
i32 factorise(i32 n);
int canRead(int fd, int seconds, int microseconds);
int newSharedMemory (unsigned int size);
void removeNewLine(char* inStr);
void printNum(i32 num);

#endif
