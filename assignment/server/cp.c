#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include "../lib.h"
#include "../../01StringAndList/String.h"

int put(int argc, char *argv[], char* request, String* response) {
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

    struct stat foutAttributes;
    char* fileData = NULL;

    if ((fileData = nextLine(request)) == NULL) {
        strConcatCS(response, "Data not found!");
        return 1;
    }

    saveToFile("test.txt", fileData, fflag, response);
    return 0;







 //TEST
    // char *dirc, *basec, *bname, *dname;
    // char *path = "/etc/passwd";

    // dirc = strdup(path);
    // basec = strdup(path);
    // dname = dirname(dirc);
    // bname = basename(basec);
    // printf("dirname=%s, basename=%s\n", dname, bname);


    char* inFilePath = argv[0]; // from client POV
    char* outFilePath = NULL; // from server POV
    if (argc == 2) {
        outFilePath = strdup(argv[1]);
    } else {
        outFilePath = "name";
        // strcpy(outFilePath, basename(inFilePath));
    }

    // size_t outFilePathLen = strlen(inFilePath);
    // if (argc == 2) {
    //     outFilePathLen = strlen(inFilePath) + strlen(argv[1]);
    // }
    // char outFilePath[outFilePathLen];
    // if (argc == 2) {
    //     strcpy(outFilePath, argv[1]);
    // } else if (argc == 1) {
    //     strcpy(outFilePath, basename(&inFilePath));
    // }





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
                // strConcatCS(out, "File already exists, file not written!\n");
                return 2;
            // }
        }
    }

    // if (copy(inFilePath, outFilePath) != 0) {
    //     strConcatCS(out, "File I/O error!\n");
    //     return 1;
    // }
    return 0;
}
