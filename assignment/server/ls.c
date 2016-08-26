#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int listdir(const char path[]) {

    DIR *dir;
    struct dirent *file;
    // struct stat attributes;
    // char buf[5120];

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "ls: can’t open %s\n", path);
    } else {
        while((file = readdir(dir)) != NULL)
        // sprintf(buf, "%s/%s", path, file->d_name);
        // stat(buf, &attributes);
        // printf("%llu", attributes.st_size);
        printf(" %s\n", file->d_name);
    }
    closedir(dir);
    return 0;
}
