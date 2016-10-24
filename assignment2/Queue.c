#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

int __QueueIsEmpty(Queue *self)
{
    return (self->size <= 0);
}

unsigned int __QueueLength(Queue *self)
{
    return (self->size);
}

void __QueueDelete(Queue *self)
{
    if (__QueueIsEmpty(self)) {
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

void __QueuePush(Queue *self, void *data)
{
    Node *newNode = malloc(sizeof(Node)); // create a new node
    newNode->data = data;
    newNode->next = NULL;

    if (__QueueIsEmpty(self)) {
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

void * __QueuePop(Queue * self)
{
    if (__QueueIsEmpty(self)) {
        return NULL;
    }

    Node *front = self->root;

    self->root = self->root->next;
    self->size--;
    return front->data;
}

//--- print out the content of the queue---
void __QueuePrintPtr(Queue *self)
{
    Node *temp = self->root;
    while (temp != NULL) {
        printf("%p ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void __QueuePrintInt(Queue *self)
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
    q.pop = __QueuePop;
    q.push = __QueuePush;
    q.destroy = __QueueDelete;
    q.length = __QueueLength;
    q.isEmpty = __QueueIsEmpty;
    q.print = __QueuePrintPtr;
    q.printInt = __QueuePrintInt;
    return q;
}
