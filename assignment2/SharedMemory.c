#include "SharedMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

static void __freeSharedMemory(SharedMemory *self) {
    if (shmctl(self->id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }
    self->id = 0;
    self->size = 0;
    self->data = NULL;
    self->data32 = NULL;
    self->dataInt = NULL;
}

static void __attachPointerSharedMemory(SharedMemory *self) {

    // SHM_RDONLY = read only
    // SHM_RND = round down to SHMLBA. Otherwise shmaddr must be a page-aligned address at which the attach occurs (1024, pagesize = 4096).
    // For the current implementation the SHMLBA value is PAGE_SIZE

    self->data = shmat(self->id, NULL, SHM_RND);
    if (shmdt(self->data) == -1) {
        fprintf(stderr, "failed to attach\n");
        perror("shmat");
        exit(-1);
    }
    self->data32 = self->data;
    self->dataInt = self->data;
}

static void __print32SharedMemory(SharedMemory *self) {
    for (int i = 0; i < self->size; ++i) {
        printf("%p -> %d\n", self->data32+i, self->data32[i]);
        printf("%p\n", self->data32+i);
    }
}

SharedMemory newSharedMemory(size_t size) {
    SharedMemory shm;
    shm.size = size;
    shm.data = NULL; // need to initialise manually, using attach
    shm.data32 = NULL; // need to initialise manually, using attach
    shm.dataInt = NULL; // need to initialise manually, using attach
    shm.id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shm.id < 0) {
        fprintf(stderr, "failed to create shared memory segment\n");
        perror("shmget");
        exit(-1);
    }

    shm.free = __freeSharedMemory;
    shm.attach = __attachPointerSharedMemory;
    shm.print = __print32SharedMemory;

    return shm;
}
