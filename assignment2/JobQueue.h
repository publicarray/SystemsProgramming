#ifndef QUEUE_H
#define QUEUE_H
// linked list

#include "Job.h"

typedef struct Node_
{
    Job j;
    struct Node_ *next;
} Node;

typedef struct JobQueue_
{
    unsigned int size;
    Node *root;

    Job (*pop)(struct JobQueue_ *self);
    void (*push)(struct JobQueue_ *self, Job j);
    void (*destroy)(struct JobQueue_ *self);
    unsigned int (*length)(struct JobQueue_ *self);
    int (*isEmpty)(struct JobQueue_ *self);
    void (*print)(struct JobQueue_ *self);
} JobQueue;

JobQueue newJobQueue();

#endif
