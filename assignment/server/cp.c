#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include "../lib.h"
#include "../../01StringAndList/String.h"

int put(int argc, char *argv[], char* fileData, String* response) {
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
                strConcatCS(response, "invalid option: -");
                strConcatC(response, optopt);
                strConcatC(response, '\n');
        }
    }
    argv += optind;
    argc -= optind;

    if (!(argc == 1 || argc == 2)) {strConcatCS(response, "usage: put [-f] source_file target_file\n"); return 1;} // if missing parameters

    if (fileData == NULL) {
        strConcatCS(response, "Data not found!");
        return 1;
    }

    // construct the file name
    char* filePath = basename(argv[0]);
    if (argc == 2) {
        filePath = argv[1];
    }

    saveToFile(filePath, fileData, fflag, response);
    return 0;
}
