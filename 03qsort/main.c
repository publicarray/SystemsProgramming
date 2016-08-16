#include <stdio.h>
#include <stdlib.h>

// http://en.cppreference.com/w/c/algorithm/qsort
int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 > arg2) return -1;
    if (arg1 < arg2) return 1;
    return 0;

    // return  (arg1 < arg2) - (arg1 > arg2); // possible shortcut
}

int main(int argc, char const *argv[]) {
    int i;
    int sum = 0;
    int size = argc -1;
    int nums[argc];

    if (argc <= 1) {printf("usage: %s <integers>\nPlease enter integers separated by spaces\n", argv[0]); return 1;}

    int temp = 0;
    for (i = 0; i < size; i++) {
        sscanf(argv[i+1], "%i", &temp); // convert to integer
        nums[i] = temp;
        sum += temp;
    }

    qsort(nums, size, sizeof(int), compare_ints);

    for (i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }

    printf("= %d\n", sum);
    return 0;
}
