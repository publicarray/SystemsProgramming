#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int comp(const void* a, const void* b)
{
    char arg1 = *(const int*)a;
    char arg2 = *(const int*)b;

    if (arg1 > arg2) return -1;
    if (arg1 < arg2) return 1;
    return 0;
}


int listdir(const char path[]) {
    DIR *dir;
    struct dirent *file;

    char list [2000][2000];
    int count = 0;
    if ((dir = opendir(path)) == NULL) {
        printf("ls: can’t open %s\n", path);
    } else {
        while((file = readdir(dir)) != NULL) {
            strcpy(list[count], file->d_name);
            // printf(" %s\n", file->d_name);
            count++;
        }

        qsort(list, count, sizeof(char[2000]), comp);

        for(int i = 0; count > i; i++) {
            printf("%s\n", list[i]);
        }
    }

    closedir(dir);
    return 0;
}
