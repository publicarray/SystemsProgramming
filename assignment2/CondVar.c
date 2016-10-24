#include "CondVar.h"
#include "Mutex.h"

static void __freeCondVar(CondVar *self)
{
    pthread_cond_destroy(&self->id);
}

static void __signalCondVar(CondVar *self)
{
    pthread_cond_signal(&self->id);
}


static void __waitCondVar(CondVar *self, Mutex *mutex)
{
    pthread_cond_wait(&self->id, &mutex->id);
}


static void __broadcastCondVar(CondVar * self)
{
    pthread_cond_broadcast(&self->id);
}

CondVar newCondVar()
{
    CondVar cv;
    pthread_cond_init(&cv.id, NULL);
    cv.free = __freeCondVar;
    cv.signal = __signalCondVar;
    cv.wait = __waitCondVar;
    cv.broadcast = __broadcastCondVar;

    return cv;
}
