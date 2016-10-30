#ifndef REQUEST_H
#define REQUEST_H

#include <sys/time.h>
#include "lib.h"

typedef struct Request_{
    i32 number;
    struct timespec startTime;
    void (*printDuration)(struct Request_ *self);
    float (*duration)(struct Request_ *self);
} Request;

Request newRequest(i32 number);

#endif
