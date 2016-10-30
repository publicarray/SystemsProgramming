/**
 * @file ConditionVariable.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A POSIX Abstraction for Condition Variables.
 */
#ifndef CONDVAR_H
#define CONDVAR_H

#include <pthread.h>
#include "Mutex.h"

// condition variable
typedef struct ConditionVariable_
{
    pthread_cond_t id;

    void (*free) (struct ConditionVariable_ *self);
    void (*wait) (struct ConditionVariable_ *self, struct Mutex_ *mutex);
    void (*signal) (struct ConditionVariable_ *self);
    void (*broadcast) (struct ConditionVariable_ *self);

} ConditionVariable;

ConditionVariable newConditionVariable();

#endif
