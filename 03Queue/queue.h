#define QSIZE 40

#ifdef QUEUE_H
#define QUEUE_H

int compare_ints(const void* a, const void* b);
int QueueAdd(int a, int n);
int QueueDel(int a, int n);
int QueueDepth();
void QueuePrint();

#endif
