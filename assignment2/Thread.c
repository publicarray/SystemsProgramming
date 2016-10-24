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

Thread newThread() {
    Thread t;
    t.startDetached = __detachedThread;
    t.start = __startThread;
    t.join = __joinThread;
    return t;
}
