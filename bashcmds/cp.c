#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#define BUFFER_SIZE 16384

struct stat foutAttributes;

int copy(const char* inFilePath, const char* outFilePath) {
    FILE *fin, *fout; // input and output files
    char buffer[BUFFER_SIZE];
    int count;

    fin = fopen(inFilePath, "rb");
    fout = fopen(outFilePath, "wb");

    if (fin == NULL) { // check that in file exists
        fclose(fin);
        fclose(fout);
        perror(inFilePath);
        return 1;
    }

    // copy from fin to fout
    while ((count = fread(buffer, 1, BUFFER_SIZE, fin)) > 0) {
        fwrite(buffer, 1, count, fout);
    }

    // other IO errors
    if (ferror(fin)) {
        puts("I/O error while reading");
    }
    if (ferror(fout)) {
        puts("I/O error while writing");
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char const *argv[]) {

    if (argc <= 2) {printf("usage: %s source_file target_file\n", argv[0]); return 1;} // if missing parameters

    const char* inFilePath = argv[1];
    const char* outFilePath = argv[2];

    int outFileExists = stat(outFilePath, &foutAttributes); // 0=yes -1=no

    if (outFileExists == 0) { // file exists
        if (S_ISDIR(foutAttributes.st_mode)) { // check if the path is a directory
            // char temp [400];
            if (strrchr(inFilePath, '/')) {
                // char* outFilePath = outFilePath + strrchr(inFilePath, '/')+1
            } else {
                // outFilePath + inFilePath;
            }
            puts("Please specify a file name.");
            return 1;
        } else {
            char option[1]; // ask do you want to override the file?
            printf("overwrite %s? (y/n [n]): ", outFilePath);
            scanf("%1c", option);
            if (tolower(*option) != 'y') { // do not overwrite unless the the user approved
                puts("not overwritten");
                return 0;
            }
        }
    }

    return copy(inFilePath, outFilePath);
}
