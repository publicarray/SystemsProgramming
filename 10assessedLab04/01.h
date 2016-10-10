#ifndef A01_H
#define A01_H
#include <pthread.h>
typedef pthread_t T_ID;

//a thread function type
typedef void*(*ThreadFunction)(void *);


typedef struct Thread_
{
    T_ID id;
    void (*threadDetached)(struct Thread_ * self, ThreadFunction f, void * data); //start thread detatched (don't call join)
    void (*start)(struct Thread_ * self, ThreadFunction f, void * data); //start regular thread (call join to get return value and clean up memory)
    void * (*join)(struct Thread_ * self); //wait for thread to finish and get the return value
} Thread;

Thread newThread();

typedef struct Point_
{
    double x;
    double y;
} Point;

#endif
