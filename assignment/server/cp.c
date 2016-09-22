#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../lib.h"
#include "../../01StringAndList/String.h"

int put(int argc, char *argv[], String* out) {
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
    if (argc != 2) {strConcatCS(out, "usage: put [source_file] [target_file]\n"); return 1;} // if missing parameters

    struct stat foutAttributes;
    char* inFilePath = argv[0];

    size_t outFilePathLen = strlen(inFilePath) + strlen(argv[1]);
    char outFilePath[outFilePathLen];
    strcpy(outFilePath, argv[1]);

    int outFileExists = stat(outFilePath, &foutAttributes); // 0=yes -1=no

    if (outFileExists == 0) { // file exists
        if (S_ISDIR(foutAttributes.st_mode)) { // check if the path is a directory
            if (strrchr(inFilePath, '/')) {
                sprintf(outFilePath, "%s%s", outFilePath, strrchr(inFilePath, '/')); // append in file name to out path
            } else {
                sprintf(outFilePath, "%s/%s", outFilePath, inFilePath); // append in file name to out path
            }
        }

        if (fflag == 0) {
            // ask do you want to override the file?
            // char option[1];
            // printf("overwrite %s? (y/n [n]): ", outFilePath);
            // scanf("%1c", option);
            // if (tolower(*option) != 'y') { // do not overwrite unless the user approved
                // puts("not overwritten");
                strConcatCS(out, "File already exists, file not written!\n");
                return 2;
            // }
        }
    }

    if (copy(inFilePath, outFilePath) != 0) {
        strConcatCS(out, "File I/O error!\n");
        return 1;
    }
    return 0;
}
