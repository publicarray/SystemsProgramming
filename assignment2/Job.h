#ifndef JOB_H
#define JOB_H

#include "lib.h"

typedef struct Job_{
    int id;
    i32 data;
    i32 output;
    int done;
    void (*print) (struct Job_ * self);

} Job;

Job newJob(int id, int data);

#endif
