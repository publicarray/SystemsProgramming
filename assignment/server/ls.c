#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
// #include <errno.h> // errno
#include <sys/stat.h>
#include "../../01StringAndList/String.h"

int __list(const char path[], String* out) {

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
            sprintf(buf, "  %s\n", file->d_name);
            strConcatCS(out, buf);
        }
    }
    // rewinddir(dir);
    if (closedir(dir) == -1) {
        perror("ls.c");
    }
    return 0;
}

int list(int argc, char *argv[], String* out) {
    // reset getopt
    optreset = 1;
    optind = 1;

    // opterr = 0; // disable getopt's own error messages e.g. case '?'
    int c, fflag = 0, lflag = 0;

    while ((c = getopt(argc, argv, "fl")) != EOF) {
        switch (c) {
            case 'f':
                fflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case '?':
                strConcatCS(out, "invalid option: -");
                strConcatC(out, optopt);
                strConcatC(out, '\n');
                // out->length += sprintf(out->data + out->length, "invalid option: -%c\n", optopt);
        }
    }

    if (fflag) printf("Option f is set\n");
    if (lflag) printf("Option l is set\n");

    argc -= optind;

    if (argc > 0) {
        int status = 0;
        for (int i = 0; i < argc; i++) {
            strConcatCS(out, argv[i+1]);
            strConcatCS(out, ":\n");
            status = __list(argv[i+1], out);
        }
        return status;
    } else {
        strConcatCS(out, ".:\n");
        return __list(".", out);
    }

}
