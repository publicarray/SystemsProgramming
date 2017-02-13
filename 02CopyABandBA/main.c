#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MYSIZE 512// 1024 / 256

void CopyAB (int* src[MYSIZE][MYSIZE], int* dest[MYSIZE][MYSIZE]) {
    int row, col;
    for(row = 0; row < MYSIZE; row++) {
        for(col = 0; col < MYSIZE; col++) {
           dest[row][col] = src[row][col];
       }
    }
}

void CopyBA (int* src [MYSIZE][MYSIZE], int* dest [MYSIZE][MYSIZE]) {
    int row, col;
    for(row = 0; row < MYSIZE; row++) {
        for(col = 0; col < MYSIZE; col++) {
           dest[col][row] = src[col][row];
        }
    }
}

int main() {
    clock_t t1, t2;
    int* src[MYSIZE][MYSIZE];
    int* dest[MYSIZE][MYSIZE];

    t1 = clock();
    CopyAB(src, dest);
    t2 = clock();
    printf("copyAB - start:%ld end:%ld duration: %ld\n", t1, t2, (t2 - t1));

    t1 = clock();
    CopyBA(src, dest);
    t2 = clock();
    printf("copyBA - start:%ld end:%ld duration: %ld\n", t1, t2, (t2 - t1));
    return 0;
}
