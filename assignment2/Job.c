#include <stdio.h>
#include "Job.h"
#include "lib.h"

static void __printJob(Job *self) {
    printf("job #%d, data:%u, output:\n", self->slot, self->data);
}

Job newJob(int slot, i32 data) {
    Job j;
    j.slot = slot;
    j.data = data;
    j.print = __printJob;
    return j;
}
