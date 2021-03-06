/**
 * @file Queue.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief Simple Linked List data structure
 * @note Currently not in use
 */

#ifndef QUEUE_H
#define QUEUE_H
// linked list

typedef struct Node_
{
    void *data;
    struct Node_ *next;
} Node;

typedef struct Queue_
{
    unsigned int size;
    Node *root;

    void * (*pop)(struct Queue_ *self);
    void (*push)(struct Queue_ *self, void *data);
    void (*destroy)(struct Queue_ *self);
    unsigned int (*length)(struct Queue_ *self);
    int (*isEmpty)(struct Queue_ *self);
    void (*print)(struct Queue_ *self);
    void (*printInt)(struct Queue_ *self);
} Queue;

Queue newQueue();

#endif
