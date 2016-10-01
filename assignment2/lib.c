#include <stdio.h>
#include <stdlib.h>
// start with how to bit shift in a cycle, threads and then inter-process/thread communications
// after that you need to look into how to build the state bard

// https://en.wikipedia.org/wiki/Circular_shift
/*
 * Shift operations in C are only defined for shift values which are
 * not negative and smaller than sizeof(value) * CHAR_BIT.
 * The mask, used with bitwise-and (&), prevents undefined behaviour
 * when the shift count is 0 or >= the width of unsigned int.
 */

#include <stdint.h>  // for uint32_t, to get 32bit-wide rotates, regardless of the size of int.
#include <limits.h>  // for CHAR_BIT

uint32_t rotl32 (uint32_t value, unsigned int count) {
    const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value<<count) | (value>>( (-count) & mask ));
}

uint32_t rotr32 (uint32_t value, unsigned int count) {
    const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value>>count) | (value<<( (-count) & mask ));
}

// Trial Division
// http://www.connellybarnes.com/documents/factoring.pdf

#include <math.h>  // used for sqrt and floor

int factorise(int n) {
    for (int i = 2; i < floor(sqrt(n)); i++) {
        if (n%i == 0) {
            // i = biggest prime found
            return i; // n/i result
        }
    }
    return 0;
}



int main(int argc, char const *argv[])
{
    printf("%d\n", rotl32(32, 3));
    printf("%d\n", factorise(1));
    return 0;
}
