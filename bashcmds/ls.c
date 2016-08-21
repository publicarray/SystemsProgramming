#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
int listdir(const char path[]) {
    DIR *dir;
    struct dirent *file;
    // struct stat attributes;
    // char buf[5120];

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "ls: can’t open %s\n", path);
    } else {
        while((file = readdir(dir)) != NULL)
        // sprintf(buf, "%s/%s", path, file->d_name);
        // stat(buf, &attributes);
        // printf("%llu", attributes.st_size);
        printf(" %s\n", file->d_name);
    }
    closedir(dir);
    return 0;

}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        return listdir(".");
    } else {
        while (--argc) {
            printf("%s:\n", *++argv);
            listdir(*argv);
        }
    }
    return 0;
}
