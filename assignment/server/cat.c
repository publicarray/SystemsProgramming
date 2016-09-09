#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#define BUFFER_SIZE 16384 // read this many bytes at a time

struct stat fAttributes;

int dump(const char* filePath) {

    if (stat(filePath, &fAttributes) == 0 && S_ISDIR(fAttributes.st_mode)) { // file is a dictionary
        return 1;
    }


    FILE* file;
    char buffer[BUFFER_SIZE];
    size_t bytesIn, bytesOut;

    file = fopen(filePath, "rb");

    if (file == NULL) { // check that in file exists
        fclose(file);
        perror(filePath);
        return errno;
    }

    // copy file to stdout
    while ((bytesIn = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        bytesOut = fwrite(buffer, 1, bytesIn, stdout);
        if (bytesIn != bytesOut) { // check that the same number of bytes are read & written
            fclose(file);
            perror("cat.c, Fatal write error");
            return errno;
        }
    }

    // other IO errors
    if (ferror(file)) {
        fclose(file);
        perror("cat.c");
        return errno;
    }
    return 0;
}
