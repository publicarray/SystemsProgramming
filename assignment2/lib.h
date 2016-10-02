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

// #include "../01StringAndList/String.h"
#include <stdint.h>

uint32_t rotl32 (uint32_t value, unsigned int count);
uint32_t rotr32 (uint32_t value, unsigned int count);
int32_t factorise(int32_t n);
int canRead(int fd, int seconds, int microseconds);
int newSharedMemory (unsigned int size);
void removeNewLine(char* inStr);

#endif
