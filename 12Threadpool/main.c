#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LThread.h"

void encode(char * data, int len, int shift) {
    for (int i = 0; i < len; ++i)
    {
        int c = data[i];
        c += shift;
        if (c < 0) {
            c = 255 - c;
        }
        c %= 255;
        data[i] = (char)c;
    }
}

void decode(char * data, int len, int shift) {
    encode(data, len, -shift);
}

typedef struct {
    int id;
    char data[100];
    int shift;
    char output[100];
} Job;

void initJob(Job * j, int id, char * str, int shift) {
    strcpy (j->data, str);
    j->shift = shift;
    j->output[0] = 0;
}

void printJob(Job j) {
    printf("Job #%i, data/out/shift = (%s, %s, %i)\n", j.id, j.data, j.output, j.shift);
}

typedef struct {
    Job data [1000];
    int front, back;
} JobQueue;

JobQueue jq, out;
LMutex m;

void initQueue (JobQueue * self) {
    self->front = self->back = 0;
}

void pushQueue (JobQueue * self, Job j ) {
    m.lock(&m);
    int nextPos = (self->back +1) % 1000;
    if (nextPos == self->front) {
        return;
    }
    self->data[self->back] = j;
    self->back = nextPos;
    m.unlock(&m);
}

int isEmpty (JobQueue * self) {
    return self->front;
}

Job popQueue (JobQueue * self) {
    Job ret;
    m.lock(&m);
    if (isEmpty(self)) return ret;
    int nextFront = (self->front + 1) % 1000;
    ret = self->data[self->front];
    self->front = nextFront;
    m.unlock(&m);
    return ret;
}



typedef struct {
    LCMutex condMutex;
    int numberOfKeys;
} Semaphore;

Semaphore newSemaphore(int initialNumKeys){
    Semaphore s;
    s.condMutex = newLCMutex();
    s.numberOfKeys = initialNumKeys;
}

void waitSem(Semaphore * self) {
    self->condMutex.lock(&self->condMutex); //lock access to keys
    while(self->numberOfKeys <= 0) //whilst there are no keys
        self->condMutex.wait(&self->condMutex); //wait in a queue + free mutex m
    self->numberOfKeys -=1; //take a key (reduce the amount for others by 1)
    self->condMutex.unlock(&self->condMutex); //unlock access to key
}

void signalSem(Semaphore * self) {
    self->condMutex.lock(&self->condMutex); //lock access to keys
    self->numberOfKeys += 1; //add a key
    self->condMutex.broadcast(&self->condMutex); //wake everyone, there is a new key!
    self->condMutex.unlock(&self->condMutex); //allow access to keys
}
void signalX(Semaphore * self, int numNewKeys) {
    for (int i = 0; i < numNewKeys; i++) signalSem(self);
}

Semaphore sem;


void * worker(void * data) {
    while (1) {
        //get a job
        int hasJob = 0;
        Job j;

        waitSem(&sem);

        m.lock(&m);
        if (!isEmpty(&jq)) {
            hasJob = 1;
            j = popQueue(&jq);
        }
        m.unlock(&m);

        // do job
        if (hasJob) {
            strcpy(j.output, j.data);
            encode(j.output, strlen(j.output), j.shift);
            lsleep(200);
            m.lock(&m);
            pushQueue(&out, j);
            m.unlock(&m);
            printJob(j);
        } else {

        }
    }

    return 0;
}


int main(int argc, char const *argv[])
{
    m = newLMutex();
    sem = newSemaphore(0);

    int numThreads = 4, i;
    for (i = 0; i < numThreads; ++i)
    {
        LThread t = newLThread();
        t.startDetached(&t, worker, 0);
    }

    m.lock(&m);
    Job j1; initJob(&j1, 1, "hello", 2);
    pushQueue(&jq, j1);
    signalSem(&sem);
    Job j2; initJob(&j1, 2, "world", 2);
    pushQueue(&jq, j2);
    signalSem(&sem);
    Job j3; initJob(&j1, 3, "foobar", 2);
    pushQueue(&jq, j3);
    signalSem(&sem);
    m.unlock(&m);

    while(1)
    {
        Job j;
        int hasJobs = 0;
        m.lock(&m);
            hasJobs = !isEmpty(&out);
            if (hasJobs) j = popQueue(&out);
        m.unlock(&m);
        if (hasJobs)
        {
            printJob(j);
        }
    }




    return 0;
}
