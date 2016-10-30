/**
 * @file Mutex.c
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A POSIX abstraction for Mutexes.
 */

#include "Mutex.h"
#include <pthread.h>

static Mutex __initMutex()
{
    Mutex m;
    pthread_mutex_init(&m.id, NULL);
    return m;
}

static void __freeMutex(Mutex * self)
{
    pthread_mutex_destroy(&self->id);
}


static void __lockMutex(Mutex * self)
{
    pthread_mutex_lock(&self->id);
}


static void __unlockMutex(Mutex * self)
{
    pthread_mutex_unlock(&self->id);
}

Mutex newMutex()
{
    Mutex m = __initMutex();
    m.free = __freeMutex;
    m.lock = __lockMutex;
    m.unlock = __unlockMutex;
    return m;
}
