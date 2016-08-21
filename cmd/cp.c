#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#define BUFFER_SIZE 16384 // read this many bytes at a time

struct stat foutAttributes;

int copy(const char* inFilePath, const char* outFilePath) {
    FILE *fin, *fout; // input and output files
    char buffer[BUFFER_SIZE];
    size_t bytesIn, bytesOut;

    fin = fopen(inFilePath, "rb");
    fout = fopen(outFilePath, "wb");

    if (fin == NULL) { // check that in file exists
        fclose(fin);
        fclose(fout);
        perror(inFilePath);
        return errno;
    }

    // copy from fin to fout
    while ((bytesIn = fread(buffer, 1, BUFFER_SIZE, fin)) > 0) {
        bytesOut = fwrite(buffer, 1, bytesIn, fout);
        if (bytesIn != bytesOut) { // check that the same number of bytes are read & written
            fclose(fin);
            fclose(fout);
            perror("Fatal write error");
            // TODO: cleanup in the event of error
            return errno;
        }
    }

    // other IO errors
    if (ferror(fin)) {
        fclose(fin);
        fclose(fout);
        perror("");
        return errno;
    }
    if (ferror(fout)) {
        fclose(fin);
        fclose(fout);
        perror("");
        return errno;
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

// TODO: add -f flag
int main(int argc, char const *argv[]) {

    if (argc != 3) {printf("usage: %s source_file target_file\n", argv[0]); return 1;} // if missing parameters

    const char* inFilePath = argv[1];

    size_t outFilePathLen = strlen(inFilePath) + strlen(argv[2]);
    char outFilePath[outFilePathLen];
    strcpy(outFilePath, argv[2]);

    int outFileExists = stat(outFilePath, &foutAttributes); // 0=yes -1=no

    if (outFileExists == 0) { // file exists
        if (S_ISDIR(foutAttributes.st_mode)) { // check if the path is a directory
            if (strrchr(inFilePath, '/')) {
                sprintf(outFilePath, "%s%s", outFilePath, strrchr(inFilePath, '/')); // append in file name to out path
            } else {
                sprintf(outFilePath, "%s/%s", outFilePath, inFilePath); // append in file name to out path
            }
        }
        // ask do you want to override the file?
        char option[1];
        printf("overwrite %s? (y/n [n]): ", outFilePath);
        scanf("%1c", option);
        if (tolower(*option) != 'y') { // do not overwrite unless the user approved
            puts("not overwritten");
            return 0;
        }
    }

    return copy(inFilePath, outFilePath);
}
