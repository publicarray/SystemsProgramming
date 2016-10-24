#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

typedef struct SharedMemory_
{
    int id;
    void *data;

    void (*free)(SharedMemory_ *self);
    void * (*getData)(SharedMemory_ *self);

} SharedMemory;

SharedMemory newSharedMemory(size_t size);

#endif


// #ifndef THREAD_H
// #define THREAD_H

// #include <pthread.h>

// typedef struct Thread_
// {
//     pthread_t id;
//     void (*startDetached)(struct Thread_ * self, void* (*f) (void *), void * data); //start thread detatched (don't call join)
//     void (*start)(struct Thread_ * self, void* (*f) (void *), void * data); //start regular thread (call join to get return value and clean up memory)
//     void * (*join)(struct Thread_ * self); //wait for thread to finish and get the return value
// } Thread;

// Thread newThread();

// #endif
