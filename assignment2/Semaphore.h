/**
 * @file Semaphore.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief An abstraction for Semaphores. Requires Mutex.c and ConditionVariable.c
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "Mutex.h"
#include "ConditionVariable.h"

typedef struct Semaphore_
{
    Mutex mutex;
    ConditionVariable condVar;
    int numberOfKeys;

    void(*wait)(struct Semaphore_ *self);
    void(*signal)(struct Semaphore_ *self);
    void(*signalX)(struct Semaphore_ *self, int numNewKeys);
    void(*free)(struct Semaphore_ *self);
} Semaphore;

Semaphore newSemaphore(int initialNumKeys);

#endif
