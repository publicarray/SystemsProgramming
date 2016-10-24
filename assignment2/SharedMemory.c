#include "SharedMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

static void __freeSharedMemory(SharedMemory *self) {
    if (shmctl(self->id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }
}

static void __updatePointerSharedMemory(SharedMemory *self) {
    self->data = shmat(self->id, NULL, 0);
}

static uint32_t * __getAsNumSharedMemory(SharedMemory *self) {
    return (uint32_t *)(self->data);
}

SharedMemory newSharedMemory(size_t size) {
    SharedMemory shm;
    shm.id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shm.id == -1) {
        perror("shmget");
        exit(1);
    }
    __updatePointerSharedMemory(&shm);

    shm.free = __freeSharedMemory;
    shm.update = __updatePointerSharedMemory;
    shm.getNum = __getAsNumSharedMemory;

    return shm;
}
