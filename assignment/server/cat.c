#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../../01StringAndList/String.h"
#include <unistd.h>
#define BUFFER_SIZE 16384 // read this many bytes at a time


int __get(const char* filePath, int fflag, String *out) {
    struct stat fAttributes;

    if (stat(filePath, &fAttributes) == 0 && S_ISDIR(fAttributes.st_mode)) { // file is a dictionary
        return 1;
    }

    FILE* file;
    char buffer[BUFFER_SIZE];
    size_t bytesIn, bytesOut;

    file = fopen(filePath, "rb");

    if (file == NULL) { // check that in file exists
        fclose(file);
        strConcatCS(out, (char *)filePath);
        strConcatCS(out, ": Unable to read file!\n");
        perror(filePath);
        return errno;
    }

    // copy file to stdout
    while ((bytesIn = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (out == NULL) {
            bytesOut = fwrite(buffer, 1, bytesIn, stdout);
            if (bytesIn != bytesOut) { // check that the same number of bytes are read & written
                fclose(file);
                perror("cat.c, I/O write error");
                strConcatCS(out, "I/O write error!\n");
                return errno;
            }
        } else {
            strConcatCS(out, buffer);
            strConcatC(out, '\n');
            if (strLength(out) == 0) {
                strConcatCS(out, "Unable to read data!\n");
            }
        }
    }

    fclose(file);
    // other IO errors
    if (ferror(file)) {
        strConcatCS(out, "I/O error!\n");
        perror("cat.c");
        return errno;
    }
    return 0;
}

int get(int argc, char *argv[], String* out) {
    // reset getopt
    optreset = 1;
    optind = 1;

    int c, fflag = 0;
    while ((c = getopt(argc, argv, "f")) != EOF) {
        switch (c) {
            case 'f':
                fflag = 1;
                break;
            case '?':
                strConcatCS(out, "invalid option: -");
                strConcatC(out, optopt);
                strConcatC(out, '\n');
        }
    }
    argv += optind;
    argc -= optind;
// DEBUG
    printf("argc: %d\n", argc);
    if (!(argc == 1 || argc == 2)) {strConcatCS(out, "usage: get [-f] source_file target_file\n"); return 1;} // if missing parameters
    return __get(argv[0], fflag, out);
}
