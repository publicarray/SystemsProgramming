/**
 * @file Thread.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A POSIX Abstraction for Threads.
 */
#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

typedef struct Thread_
{
    pthread_t id;
    void (*startDetached)(struct Thread_ * self, void* (*f) (void *), void * data); //start thread detatched (don't call join)
    void (*start)(struct Thread_ * self, void* (*f) (void *), void * data); //start regular thread (call join to get return value and clean up memory)
    void * (*join)(struct Thread_ * self); //wait for thread to finish and get the return value
    void (*kill)(struct Thread_ * self); //kill thread
} Thread;

Thread newThread();

#endif
