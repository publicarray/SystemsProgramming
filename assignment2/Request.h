/**
 * @file Request.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief An object for storing job parameters for the client i.e. the request to the server
 */

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
