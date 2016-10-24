#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

static int __isEmptyQueue(Queue *self)
{
    return (self->size <= 0);
}

static unsigned int __lengthQueue(Queue *self)
{
    return (self->size);
}

static void __deleteQueue(Queue *self)
{
    if (__isEmptyQueue(self)) {
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

static void __pushQueue(Queue *self, void *data)
{
    Node *newNode = malloc(sizeof(Node)); // create a new node
    newNode->data = data;
    newNode->next = NULL;

    if (__isEmptyQueue(self)) {
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

static void * __popQueue(Queue * self)
{
    if (__isEmptyQueue(self)) {
        return NULL;
    }

    Node *front = self->root;

    self->root = self->root->next;
    self->size--;
    return front->data;
}

//--- print out the content of the queue---
static void __printPtrQueue(Queue *self)
{
    Node *temp = self->root;
    while (temp != NULL) {
        printf("%p ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

static void __printIntQueue(Queue *self)
{
    Node *temp = self->root;
    while (temp != NULL) {
        printf("%i ", (int) temp->data);
        temp = temp->next;
    }
    printf("\n");
}

Queue newQueue()
{
    Queue q;
    q.size = 0;
    q.root = NULL;
    q.pop = __popQueue;
    q.push = __pushQueue;
    q.destroy = __deleteQueue;
    q.length = __lengthQueue;
    q.isEmpty = __isEmptyQueue;
    q.print = __printPtrQueue;
    q.printInt = __printIntQueue;
    return q;
}
