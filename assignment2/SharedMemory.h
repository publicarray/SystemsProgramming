#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

typedef struct SharedMemory_
{
    int id;
    unsigned int size;
    void *data;
    int *dataInt;
    uint32_t *data32;

    void (*free)(struct SharedMemory_ *self);
    void * (*attach)(struct SharedMemory_ *self);
    void (*print)(struct SharedMemory_ *self);

} SharedMemory;

SharedMemory newSharedMemory(size_t size);

#endif
