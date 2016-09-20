#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
// #include <errno.h> // errno
#include <sys/stat.h>
#include "../../01StringAndList/String.h"

int __list(const char path[], int lflag, String* out) {

// The long listing option of the list commands will also return the file size, owner, creation date and access permissions.

    DIR *dir;
    struct dirent *file;
    struct stat fst;
    struct timespec dateTime;
    char buf[5120];
    char filepath[500];
    int status = 0;

    if ((dir = opendir(path)) == NULL) {
        sprintf(buf, "Can not open directory: %s\n", path);
        strConcatCS(out, buf);
        return 1;
    }

    while((file = readdir(dir)) != NULL) {
        sprintf(filepath, "%s/%s", path, file->d_name);
        if (lflag) { // list long information if -l flag is set
            status = lstat(filepath, &fst); // get file attributes
            if (status == 0) {
                dateTime = fst.st_mtimespec;
                sprintf(buf, "%5d %5d %7llu %s\n", fst.st_uid, fst.st_gid, fst.st_size, file->d_name);
            }
        } else {
            sprintf(buf, "  %s\n", file->d_name);
        }
        strConcatCS(out, buf);
    }

    if (closedir(dir) == -1) {
        perror("ls.c");
        return 1;
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

    argv += optind;
    argc -= optind;

    if (argc > 0) {
        int status = 0;
        for (int i = 0; i < argc; i++) {
            strConcatCS(out, argv[i]);
            strConcatCS(out, ":\n");
            status = __list(argv[i], lflag, out);
        }
        return status;
    } else {
        strConcatCS(out, ".:\n");
        return __list(".", lflag, out);
    }

}
