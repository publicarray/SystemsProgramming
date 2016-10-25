#include <stdio.h>
#include <stdlib.h>
#include "JobQueue.h"
#include "Job.h"

static int __isEmptyJobQueue(JobQueue *self)
{
    return (self->size <= 0);
}

static unsigned int __lengthJobQueue(JobQueue *self)
{
    return (self->size);
}

static void __deleteJobQueue(JobQueue *self)
{
    if (__isEmptyJobQueue(self)) {
        return;
    }

    Node *ptr = self->root;
    while (ptr != NULL) {
        Node *temp = ptr;
        ptr = ptr->next;
        free(temp);
    }

    self->size = 0;
    self->root = NULL;
}

static void __pushJobQueue(JobQueue *self, Job j)
{
    Node *newNode = malloc(sizeof(Node)); // create a new node
    newNode->j = j;
    newNode->next = NULL;

    if (__isEmptyJobQueue(self)) {
        self->root = newNode;
    } else {
        Node *temp = self->root;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    self->size++;
}

static Job __popJobQueue(JobQueue * self)
{
    if (__isEmptyJobQueue(self)) {
        return newJob(0,0);
    }

    Node *front = self->root;

    self->root = self->root->next;
    self->size--;
    return front->j;
}

//--- print out the content of the queue---
static void __printJobQueue(JobQueue *self)
{
    Node *temp = self->root;
    while (temp != NULL) {
        printf("1");
        temp = temp->next;
    }
    printf("\n");
}

JobQueue newJobQueue()
{
    JobQueue q;
    q.size = 0;
    q.root = NULL;
    q.pop = __popJobQueue;
    q.push = __pushJobQueue;
    q.destroy = __deleteJobQueue;
    q.length = __lengthJobQueue;
    q.isEmpty = __isEmptyJobQueue;
    q.print = __printJobQueue;
    return q;
}
