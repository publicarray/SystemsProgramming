#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

struct stat fAttributes;

int rm(const char* filePath) {

    if (remove(filePath) != 0) {
        perror(filePath);
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[]) {

    if (argc != 2) {printf("usage: %s file\n", argv[0]); return 1;} // if missing parameters

    const char* filePath = argv[1];

    int fileExists = stat(filePath, &fAttributes); // 0=yes -1=no

    if (fileExists == 0) { // file exists
        if (S_ISDIR(fAttributes.st_mode)) { // check if the path is a directory
            printf("rm: %s: is a directory\n", filePath);
            return 1;
        }
        // ask do you want to remove the file?
        char option[1];
        printf("remove %s? ", filePath);
        scanf("%1c", option);
        if (tolower(*option) != 'y' && *option != '\r' && *option != '\n') { // do not remove unless the user approved
            return 0;
        }
    }

    return rm(filePath);
}
