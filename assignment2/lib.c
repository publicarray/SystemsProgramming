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
// https://en.wikipedia.org/wiki/Trial_division

// 30 = 2,3,5 // prime factorisation
int32_t factorise(int32_t n) {
    for (int32_t i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            // i = smallest factor found
            return i; // n/i result
        }
    }

    return n; // found a prime number
}

// Fermat primality test
// a^p-1 mod p = 1?
// a= a random number that is less than p
// p= int to check e.g 181
// 68^180 mod 108 = 1 so probably a prime with low error


int main(int argc, char const *argv[])
{
    printf("%d\n", rotl32(32, 3));
    printf("%d\n", factorise(30));
    printf("%d\n", factorise(54734711)); //prime:  54734711
    printf("%d\n", factorise(-10));
    printf("%d\n", factorise(-54734711));
    printf("%d\n", rotl32(32, 32-1));
    return 0;
}
