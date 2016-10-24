#include "SharedMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

static void __freeSharedMemory(SharedMemory *self) {
    if (shmctl(self->id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }
}

static void * __getDataSharedMemory(SharedMemory *self) {
    self->data = shmat(self->id, NULL, 0);
    return self->data;
}

SharedMemory newSharedMemory(size_t size) {
    SharedMemory shm;
    shm.data = 0x00;
    shm.free = __freeSharedMemory;
    shm.getData = __getDataSharedMemory;
    shm.id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shm.id == -1) {
        perror("shmget");
        exit(1);
    }

    return shm;
}
