#include "Semaphore.h"
#include "Mutex.h"
#include "ConditionVariable.h"

void __waitSemaphore(Semaphore *self) {
    self->mutex.lock(&self->mutex); //lock access to keys
    while (self->numberOfKeys <= 0) {//whilst there are no keys
        self->condVar.wait(&self->condVar, &self->mutex); //wait in a queue + free mutex m
        // pthread_cond_wait(&self->condVar.id, &self->mutex.id); //wait in a queue + free mutex m
    }
    self->numberOfKeys -=1; //take a key (reduce the amount for others by 1)
    self->mutex.unlock(&self->mutex); //unlock access to key
}

void __signalSemaphore(Semaphore *self) {
    self->mutex.lock(&self->mutex); //lock access to keys
    self->numberOfKeys += 1; //add a key
    self->condVar.broadcast(&self->condVar); //wake everyone, there is a new key!
    self->mutex.unlock(&self->mutex); //allow access to keys
}

void __signalXSemaphore(Semaphore *self, int numNewKeys) {
    for (int i = 0; i < numNewKeys; i++) {
        __signalSemaphore(self);
    }
}

Semaphore newSemaphore(int initialNumKeys) {
    Semaphore s;
    s.mutex = newMutex();
    s.condVar = newConditionVariable();
    s.numberOfKeys = initialNumKeys;

    s.wait = __waitSemaphore;
    s.signal = __signalSemaphore;
    s.signalX = __signalXSemaphore;

    return s;
}
