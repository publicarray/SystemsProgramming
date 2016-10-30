/**
 * @file Job.h
 * @author Sebastian Schmidt
 * @date 30 Oct 2016
 * @brief An object for storing job parameters
 */
#ifndef JOB_H
#define JOB_H

#include "lib.h"

typedef struct Job_{
    int slot;
    i32 data;
    void (*print) (struct Job_ * self);

} Job;

Job newJob(int id, i32 data);

#endif
