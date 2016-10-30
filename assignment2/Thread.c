/**
 * @file Thread.c
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief A POSIX Abstraction for Threads.
 */
#include "Thread.h"
#include <pthread.h>

static void __detachedThread(Thread * self, void* (*f) (void *), void * data) {
    pthread_create(&self->id, NULL, f, data);
    pthread_detach(self->id);
}

static void __startThread(Thread * self, void* (*f) (void *), void * data) {
    pthread_create(&self->id, NULL, f, data);
}


static void* __joinThread(Thread * self) {
    void* value_ptr;
    pthread_join(self->id, &value_ptr);
    return value_ptr;
}

static void __killThread(Thread * self) {
    pthread_kill(self->id, 0);
}

Thread newThread() {
    Thread t;
    t.startDetached = __detachedThread;
    t.start = __startThread;
    t.join = __joinThread;
    t.kill = __killThread;
    return t;
}
