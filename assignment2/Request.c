#include <stdio.h>
#include <sys/time.h>
#include "Request.h"
#include "lib.h"

static void __printDurationRequest(Request *self) {
    printf("Duration: %f\n", getTimeLapsed(self->startTime));
}

static float __durationRequest(Request *self) {
    return getTimeLapsed(self->startTime);
}

Request newRequest(i32 number) {
    Request r;
    r.number = number;
    r.startTime = getTime();
    r.printDuration = __printDurationRequest;
    r.duration = __durationRequest;
    return r;
}
