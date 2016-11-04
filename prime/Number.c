#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Number.h"

static void __reallocNumber(Number * self, size_t newSize) {
    if (newSize <= self->bufferSize) {
        return;
    }

    char* temp = NULL;
    if ((temp = realloc(self->data, newSize)) != NULL){
        self->data = temp;
    }
}

static void __printNumber(Number * self) {
    printf("%c", self->sign);
    for (int i = self->bufferSize - self->digits; i < self->bufferSize; ++i) {
        // printf("%i:%c|", i, self->data[i]);
        printf("%c", self->data[i]);
    }
    printf("\n");
}

// assume null terminated string
static Number __addNumber(Number * self, char * buffer) {
    size_t bufferLen = strlen(buffer);
    Number sum = initNumber(self->bufferSize + bufferLen);
    size_t i = self->digits-1;
    size_t j = bufferLen-1;
    size_t k = sum.bufferSize-1;

    char c1;
    char c2;
    int digit = 0;
    int carry = 0;
    int nextCarry = 0;

    for (; k > 0; --k) {
        if (i+1 == 0 && j+1 == 0 && carry == 0) {
            break;
        }

        if (i+1 > 0) {
            c1 = self->data[i];
            --i;
        }
        if (j+1 > 0) {
            c2 = buffer[j];
            --j;
        }

        digit = (c1 - '0') + (c2 - '0'); // https://stackoverflow.com/questions/2915725/using-atoi-with-char#2915784

        if (digit > 9) {
            nextCarry = digit/10;
            digit -= nextCarry * 10;
        }

        // printf("%li %li %li x:%c y:%c sum:%i carry:%i\n", i, j, k, c1, c2, digit, carry);
        if (carry) {
            sum.data[k] = digit + carry + '0';
            carry = 0;
        } else  {
            sum.data[k] = digit + '0';
        }
        // sprintf(&r.data[k], "%i", digit);
        // printf("%c\n", sum.data[k]);

        sum.digits++;
        carry = nextCarry;
        nextCarry = 0;
        c1 = '0';
        c2 = '0';
    }
    sum.sign = '+'; // todo
    return sum;
}

static Number __addNumberNumber(Number * self, Number * other) {
    return __addNumber(self, other->data);
}

static void __freeNumber(Number * self) {
    free(self->data);
    self->digits = 0;
    self->bufferSize = 0;
    self->sign = '+';
}

Number initNumber(size_t bufferSize) {
    Number n;
    n.data = malloc(bufferSize);
    n.bufferSize = bufferSize;
    n.digits = 0;
    n.sign = '+';

    n.free = __freeNumber;
    n.print = __printNumber;
    n.add = __addNumberNumber;

    memset(n.data, 0, n.bufferSize); // pad
    return n;
}

// assume null terminated string
Number newNumber(char * buffer) {
    Number n = initNumber(strlen(buffer));
    n.digits = n.bufferSize;
    n.sign = '+';
    if (buffer[0] == '-') {
        n.sign = '-';
        buffer++;
    }
    memcpy(n.data, buffer, n.bufferSize); // copy data
    return n;
}
