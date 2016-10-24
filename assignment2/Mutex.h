#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

typedef struct Mutex_
{
    pthread_mutex_t id;

    void (*free) (struct Mutex_ * self);
    void (*lock) (struct Mutex_ * self);
    void (*unlock) (struct Mutex_ * self);

} Mutex;

Mutex newMutex();

#endif
