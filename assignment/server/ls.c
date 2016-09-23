#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
// #include <errno.h> // errno
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <pwd.h>
#include <grp.h>
#include "../../01StringAndList/String.h"

int __list(const char path[], int lflag, String* out) {

    DIR *dir;
    struct dirent *file;
    struct stat fileStat;

    char buf[5120];
    char filepath[500];
    int status = 0;

    struct timespec dateTime;
    struct tm localDateTime;
    char dateTimeFormat[21];
    char datetimeFormatMs[26];

    struct passwd *user;
    struct group *group;
    if ((dir = opendir(path)) == NULL) {
        sprintf(buf, "Can not open directory '%s'\n", path);
        strConcatCS(out, buf);
        return 1;
    }

    while((file = readdir(dir)) != NULL) {
        sprintf(filepath, "%s/%s", path, file->d_name);
        if (lflag) { // list long information if -l flag is set
            status = lstat(filepath, &fileStat); // get file attributes
            if (status == 0) {
                // Get last modification date
                dateTime = fileStat.st_mtimespec; // get the last modification date
                localDateTime = *localtime(&dateTime.tv_sec); // convert into a tm struct
                // strftime(dateTimeFormat, sizeof dateTimeFormat, "%Y-%m-%d %H:%M:%S", &localDateTime); // format date into a string
                strftime(dateTimeFormat, sizeof dateTimeFormat, "%e %b %Y %H:%M:%S", &localDateTime); // format date into a string
                snprintf(datetimeFormatMs, sizeof datetimeFormatMs, "%s.%03ld", dateTimeFormat, dateTime.tv_nsec); // add manoseconds // OS X Extended formatted disk only store timestamps in seconds.
                // Get User
                user = getpwuid(fileStat.st_uid);
                // Get Group
                group = getgrgid(fileStat.st_gid);

                if (user && group) {
                    // Get and format Permissions
                    // https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
                    strConcatCS(out, (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IROTH) ? "r" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
                    strConcatCS(out, (fileStat.st_mode & S_IXOTH) ? "x" : "-");
                    sprintf(buf, " %-5s %-5s %7llu %s %s", user->pw_name, group->gr_name , fileStat.st_size, datetimeFormatMs, file->d_name); // print attributes of a single file
                    strConcatCS(out, buf);
                    sprintf(buf, "%s\n", (S_ISLNK(fileStat.st_mode)) ? " -> Symbolic Link" : ""); // symbolic link
                    strConcatCS(out, buf);
                }
            }
        } else {
            sprintf(buf, "%s\n", file->d_name);
            strConcatCS(out, buf);
        }
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

    argv += optind;
    argc -= optind;

    if (argc > 0) {
        int status = 0;

        strConcatC(out, '\n');
        strConcatCS(out, argv[0]);
        strConcatCS(out, ":\n");
        status = __list(argv[0], lflag, out);

        return status;
    } else {
        strConcatCS(out, ".:\n");
        return __list(".", lflag, out);
    }

}
