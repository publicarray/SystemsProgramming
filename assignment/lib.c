#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "lib.h"
#include "../01StringAndList/String.h"
#define BUF_SIZE 16384

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

struct timespec getTime() {
    struct timespec time;

    // https://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
    #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    time.tv_sec = mts.tv_sec;
    time.tv_nsec = mts.tv_nsec;
    #else
    clock_gettime(CLOCK_REALTIME, &time);
    #endif

    return time;
}

float getTimeLapsed(struct timespec start) {
    struct timespec end = getTime();
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

float getTimeNsec() {
    struct timespec time = getTime();
    return time.tv_sec + time.tv_nsec / 1000000000.0;
}

int strLen(char *string) {
    int length = 0;

    while (string[length] != '\0') {
        length++;
    }
    return length;
}

void removeNewLine(char* inStr) {
    if ((strLen(inStr) > 0) && (inStr[strLen(inStr) - 1] == '\n')) {
        inStr[strLen(inStr) - 1] = '\0';
    }
}

char* nextLine(char* inStr) {
    int length;
    if ((length = strLen(inStr)) > 0) {
        for (int i = 0; i < length-1; i++) {
            if (inStr[i] == '\n' && inStr[i+1]) { // check that there are character left after the '/n'
                return &inStr[i+1];
            }
        }
    }
    return NULL;
}

char* splitNextLine(char* inStr) {
    int length;
    if ((length = strLen(inStr)) > 0) {
        for (int i = 0; i < length-1; i++) {
            if (inStr[i] == '\n' && inStr[i+1]) { // check that there are character left after the '/n'
                inStr[i] = 0x00;
                return &inStr[i+1];
            }
        }
    }
    return NULL;
}

void strToLower(char *string) {
    for(int i = 0; string[i]; i++){
        string[i] = tolower(string[i]);
    }
}

int splitStr(char* srcString, char* tokens[], int maxTokens) {
    int numFound = 0;
    int saved = 0;
    int length = strLen(srcString);

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Maximum number of tokens reached\n");
            return numFound;
        }

// TODO: I could improve performance by stopping the search at a '\n' / or create a separate function to split the string at the first \n
        if (srcString[i] == ' ') {                  // if delimiter is found
            srcString[i] = 0x00;                    // replace with null char
            saved = 0;                              // marked as not saved (need to save on next iteration)
        } else if (saved == 0) {                    // if not saved
            tokens[numFound] = &srcString[i];       // save the address
            numFound++;
            saved = 1;                              // ignore (don't save anything)
        } else {                                    // all other characters
            saved = 1;                              // ignore (don't save anything)
        }
    }
    return numFound;
}

void printClient(char *message) {
    printf(BLU "%s" NRM, message);
}
void printServer(char *message) {
    printf(CYN "%s" NRM, message);
}

void message(char* msg, String* out) {
    puts(msg);
    if (out) {
        strConcatCS(out, msg);
        strConcatC(out, '\n');
    }
}
int saveToFile(char* to, char* bytes, int force, String* out) {
    struct stat foutAttributes;
    int outFileExists = stat(to, &foutAttributes); // 0=yes -1=no
    if (outFileExists == 0) { // file exists
        if (S_ISDIR(foutAttributes.st_mode)) { // check if the path is a directory
            message("This is a directory, please give a file name", out);
            return 1;
            // if (strrchr(inFilePath, '/')) {
            //     sprintf(to, "%s%s", to, strrchr(inFilePath, '/')); // append in file name to out path
            // } else {
            //     sprintf(to, "%s/%s", to, inFilePath); // append in file name to out path
            // }
        }
        // ask do you want to override the file?
        if (!force) {
            char option[1];
            message("File already exists!", out);
            if (out) { // exit early if we are responding request to a client, don't bother asking
                return 1;
            }
            printf("overwrite %s? (y/n [n]): ", to);
            scanf("%1c", option);
            if (tolower(*option) != 'y') { // do not overwrite unless the user approved
                puts("not overwritten");
                return 0;
            }
        }
    }

    FILE *fout = fopen(to, "wb");
    size_t writeCount = 0;

    if (!fout) {
        perror(to);
        if (out) {
            strConcatCS(out, strerror(errno));
        }
        return 1;
    }

    writeCount = fwrite(bytes, 1, strlen(bytes), fout);

    if (writeCount == 0) {
        message("No bytes written", out);
        fclose(fout);
        return 1;
    }

    if (ferror(fout)) {
        message("I/O error when writing", out);
        fclose(fout);
        return 1;
    }

    if (out) {
        strConcatCS(out, "file saved\n");
    }
    fclose(fout);
    return 0;
}

int readFile(char* filepath, String* buffer) {
    char temp[500];
    struct stat foutAttributes;
    int outFileExists = stat(filepath, &foutAttributes); // 0=yes -1=no
    if (outFileExists == -1) { // file does not exists
        puts("File does not exist");
        return 1;
    }
    if (S_ISDIR(foutAttributes.st_mode)) { // check if the path is a directory
        puts("This is a directory, please enter a file name");
        return 1;
    }

    FILE *fin = fopen(filepath, "rb");
    size_t readCount = 0;

    if (!fin) {
        perror(filepath);
        return 1;
    }

    while ((readCount = fread(temp, 1, sizeof temp, fin)) > 0) {
        temp[readCount] = 0x00;
        strConcatCS(buffer, temp);
    }

    if (ferror(fin)) {
        puts("I/O error");
        fclose(fin);
        return 1;
    }

    fclose(fin);
    return 0;
}

int copy(char* from, char* to)
{
    FILE *fin, *fout; // input and output
    char buf[BUF_SIZE]; // buffer
    int totalReadCount = 0, totalWriteCount = 0, readCount = 0, writeCount = 0;

    fin = fopen(from, "rb");
    fout = fopen(to, "wb");

    if(!fin || !fout) {
        perror("Opening file failed");
        return 1;
    }

    // copy from fin to fout
    while ((readCount = fread(buf, 1, BUF_SIZE, fin)) > 0) {
        writeCount = fwrite(buf, 1, readCount, fout);
        totalReadCount += readCount;
        totalWriteCount += writeCount;
    }

    if (totalReadCount != totalWriteCount) {
        puts("File count does not match! File is Corrupted.");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    if (ferror(fin) || ferror(fout)) {
        puts("I/O error when reading");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    // else if (feof(fin) || ferror(fout)) {
    //     puts("End of file reached successfully");
    // }

    fclose(fin);
    fclose(fout);
    return 0;
}
