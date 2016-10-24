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
    int size;
    Node *root;

    void(*pop)(Queue_ *self);
    void(*push)(Queue_ *self, void *data);
    void(*destroy)(Queue_ *self);
    void(*length)(Queue_ *self);
    void(*isEmpty)(Queue_ *self);
    void(*print)(Queue_ *self);
    void(*printInt)(Queue_ *self);
} Queue;

Queue newQueue();

#endif
