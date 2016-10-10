#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <math.h>
#include <termios.h>
#include "01.h"

#include <pthread.h>
#include <unistd.h>

static void detachedThread(Thread * self, void * (*f) (void *), void * data)
{
    pthread_create(&self->id, NULL, f, data);
    pthread_detach(self->id);
}

static void startThread(Thread * self, void * (*f) (void *), void * data)
{
    pthread_create(&self->id, NULL, f, data);
}


static void * joinThread(Thread * self)
{
    void * ret;
    pthread_join(self->id, &ret);
    return ret;
}

Thread newThread(){
    Thread ret;
    ret.threadDetached = detachedThread;
    ret.start = startThread;
    ret.join = joinThread;
    return ret;
}

long double error(long double actual, long double expected) {
    return fabsl((actual - expected))/ expected * 100;
}

int isInCircle(Point self) {
    return ((self.x - 10)*(self.x - 10) + (self.y - 10)*(self.y - 10) <= 10*10);
}

void* func(void* num) {
    int numPointsInSquare = 0;
    Point points [100];
    for (int i = 0; i < 100; ++i)
    {
        points[i].x = (rand() / (double) RAND_MAX) * 20.0;
        points[i].y = (rand() / (double) RAND_MAX) * 20.0;
        // printf("%fx%f\n", points[i].x, points[i].y);
        numPointsInSquare += isInCircle(points[i]);
    }
    // printf("%d\n", numPointsInSquare);
    *(int*)num = numPointsInSquare;
    return NULL;
}

#include <sys/shm.h>
int newSharedMemory (unsigned int size) {
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

int main(int argc, char *argv[])
{
    srand((unsigned int)pthread_self());
    opterr = 0; // disable getopt's own error messages e.g. case '?'
    int c, threadNum, bflag = 0; // port 80 requires sudo

    while ((c = getopt(argc, argv, "b:")) != EOF) {
        switch (c) {
            case 'b':
                bflag = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "invalid option: -%c\n", optopt);
        }
    }

    // skip arguments that have been processed
    argv += optind;
    argc -= optind;

    if (argv[0] == NULL) {
        printf("usage: 01 [-b] ThreadCount\n \n");
        exit(1);
    }

    threadNum = atoi(argv[0]);
    int sumShmid = newSharedMemory(sizeof(int) * threadNum);
    int *nums = shmat(sumShmid, NULL, 0);
    int sum = 0;
    Thread threads[threadNum];
    long double realPI = 4*atanl(1);
    for (int i = 0; i < threadNum; ++i)
    {
        threads[i] = newThread();
        threads[i].start(&threads[i], func, nums+i);
    }


    for (int i = 0; i < threadNum; ++i)
    {
        threads[i].join(&threads[i]);
        sum += *(nums+i);
        long double PI = 4.0 * sum / (100.0 * threadNum);
        // printf("Sum = %d\n", sum);
        printf("PI = %Lf      Error = %Lf%%\n", PI, error(PI, realPI));
        // if (bflag) {
            // BarDisplay(threadNum, i);
        // }
    }
    printf("Real PI = %.20Lf\n", realPI);
    shmctl(sumShmid, IPC_RMID, NULL);

    return 0;
}
