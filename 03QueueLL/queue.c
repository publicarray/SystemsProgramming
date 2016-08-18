#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct Node_Struct {
    int data;
    struct Node_Struct *next;
} Node;

Node *root = NULL;
int size = 0;

//--- Add to queue ---
// a any item, n = QSIZE
int QueueAdd(int a, int n)
{
    if (size >= QSIZE) {
        return 1;
    }

    Node *newNode = malloc(sizeof(Node)); // create a new node
    newNode->data = a;
    newNode->next = NULL;

    Node *temp = root;

    if (root == NULL || root->data < a) { // if it's the first item or is less than the root's data
        root = newNode;
        root->next = temp;
        size++;
        return 0;
    } else {
        while(temp != NULL) {
            if (temp->data >= a && (temp->next == NULL || temp->next->data < a)) {
                newNode->next = temp->next;
                temp->next = newNode;
                size++;
                return 0;
            }
            temp = temp->next;
        }
    }
    return 1; // this should not happen
}
//--- Remove from queue ---
int QueueDel(int a, int n)
{
    if (size <= 0) {
        return 1;
    }

    Node *temp = root;
    root = root->next;
    size--;
    free(temp);
    return 0;
}
//--- get queue depth ---
int QueueDepth()
{
    return size;
}
//--- print out the content of the queue---
void QueuePrint()
{
    Node *temp = root;
    while(temp != NULL) {
        printf("%i ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}
