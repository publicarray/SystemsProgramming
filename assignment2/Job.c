// #include "lib.h"
#include <stdio.h>
#include "Job.h"

static void __printJob(Job *self) {
    printf("job #%d, data:%u, output:\n", self->id, self->data);
}

Job newJob(int id, int data) {
    Job j;
    j.id = id;
    j.data = data;
    j.output = 0;
    j.done = 0;
    j.print = __printJob;
    return j;
}
