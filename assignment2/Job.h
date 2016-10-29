#ifndef JOB_H
#define JOB_H

#include "lib.h"

typedef struct Job_{
    int slot;
    i32 data;
    void (*print) (struct Job_ * self);

} Job;

Job newJob(int id, int data);

#endif
