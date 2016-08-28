#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "../../01StringAndList/String.h"

int listdir(const char path[], String* out) {

    DIR *dir;
    struct dirent *file;
    // struct stat attributes;
    char buf[5120];

    if ((dir = opendir(path)) == NULL) {
        sprintf(buf, "ls: can’t open %s\n", path);
        strConcatCS(out, buf);
    } else {
        while((file = readdir(dir)) != NULL) {
            // sprintf(buf, "%s/%s", path, file->d_name);
            // stat(buf, &attributes);
            // printf("%llu", attributes.st_size);
            sprintf(buf, " %s\n", file->d_name);
            strConcatCS(out, buf);
        }
    }
    // rewinddir(dir);
    if (closedir(dir) == -1) {
        perror("ls.c");
    }
    return 0;
}
