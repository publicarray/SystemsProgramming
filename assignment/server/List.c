#include <stdlib.h>
#include <string.h>
#include "List.h"

void initList(List* self) {
    self->length = 0;
    self->bufferLength = 50;
    self->data = (void* *) malloc(sizeof(void *) * self->bufferLength); // TODO: error checking
}
void* addList(List* self, void* ptr, int sizeOfInput) {
    // automatically grow and shrink the list
    if (self->length >= self->bufferLength) {
        self->bufferLength *= 2;
        self->data = (void* *) realloc(self->data, sizeof(void *) * self->bufferLength); // TODO: error checking
    }
    // copy item to the list
    void* newData = malloc(sizeOfInput);
    memcpy(newData, ptr, sizeOfInput);
    self->data[self->length] = newData;
    self->length++;
    return newData;
    // self->data[self->length++] = (void*) malloc (sizeOfInput);
    // memcpy(self->data[self->length-1], ptr, sizeOfInput);
}

void* getList(List* self, int index) {
    return self->data[index];
}

void freeList(List* self) {
    for(int i=0; i<self->length; i++) {
        free(self->data[i]);
    }
    free(self->data);
}
