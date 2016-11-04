#ifndef NUMBER_H
#define NUMBER_H

#include <stdlib.h>

typedef struct Number_{
    char* data;
    char sign; // '+' positive, '-' negative
    size_t bufferSize;
    size_t digits;
    void (*free) (struct Number_ * self);
    void (*print) (struct Number_ * self);
    struct Number_ (*add) (struct Number_ * self, struct Number_ * other);
} Number;

Number newNumber(char* buffer);
Number initNumber(size_t digits);

#endif
