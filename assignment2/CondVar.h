#ifndef CONDVAR_H
#define CONDVAR_H

#include <pthread.h>
#include "Mutex.h"

// condition variable
typedef struct CondVar_
{
    pthread_cond_t id;

    void (*free) (struct CondVar_ *self);
    void (*wait) (struct CondVar_ *self, struct Mutex_ *mutex);
    void (*signal) (struct CondVar_ *self);
    void (*broadcast) (struct CondVar_ *self);

} CondVar;

CondVar newCondVar();

#endif
