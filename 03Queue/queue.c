#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int queue[QSIZE]; //= QueueInit(QSIZE);
int size = 0;

// http://en.cppreference.com/w/c/algorithm/qsort
int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 < arg2) - (arg1 > arg2);
}

//--- Add to queue ---
// a any item, n = QSIZE
int QueueAdd(int a, int n)
{
    if (size < n) {
        queue[size] = a;
        size++;
        qsort(queue, size, sizeof(int), compare_ints);
        return 0;
    }
    return 1; // error: queue full
}
//--- Remove from queue ---
int QueueDel(int a, int n)
{
    if (size > 0) {
        queue[size] = 0;
        size--;
        return 0;
    }
    return 1;
}
//--- get queue depth ---
int QueueDepth()
{
    return size;
}
//--- print out the content of the queue---
void QueuePrint()
{
    for (int i=0; i<size; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}
