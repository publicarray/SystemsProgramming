#ifndef STHREAD_H
#define STHREAD_H

#include <pthread.h>

//a thread function type
// typedef void*(*LThreadFunction)(void *);


typedef struct SThread_
{
    pthread_t id;
    // void (*threadDetached)(struct LThread_ * self, LThreadFunction f, void * data); //start thread detatched (don't call join)
    // void (*start)(struct LThread_ * self, LThreadFunction f, void * data); //start regular thread (call join to get return value and clean up memory)
    // void * (*join)(struct LThread_ * self); //wait for thread to finish and get the return value
} SThread;

SThread newSThread();

#endif
