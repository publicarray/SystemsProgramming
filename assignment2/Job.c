// #include "lib.h"
#include <stdio.h>
#include "Job.h"

static void __printJob(Job *self) {
    printf("job #%d, data:%u, output:\n", self->slot, self->data);
}

Job newJob(int slot, int data) {
    Job j;
    j.slot = slot;
    j.data = data;
    j.print = __printJob;
    return j;
}
