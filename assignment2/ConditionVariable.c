/**
 * @file ConditionVariable.c
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A POSIX Abstraction for Condition Variables.
 */

#include "ConditionVariable.h"
#include "Mutex.h"

static void __freeCondVar(ConditionVariable *self)
{
    pthread_cond_destroy(&self->id);
}

static void __signalCondVar(ConditionVariable *self)
{
    pthread_cond_signal(&self->id);
}


static void __waitCondVar(ConditionVariable *self, Mutex *mutex)
{
    pthread_cond_wait(&self->id, &mutex->id);
}


static void __broadcastCondVar(ConditionVariable * self)
{
    pthread_cond_broadcast(&self->id);
}

ConditionVariable newConditionVariable()
{
    ConditionVariable cv;
    pthread_cond_init(&cv.id, NULL);
    cv.free = __freeCondVar;
    cv.signal = __signalCondVar;
    cv.wait = __waitCondVar;
    cv.broadcast = __broadcastCondVar;

    return cv;
}
