#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

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

void removeNewLine (char* inStr) {
    if ((strLen(inStr) > 0) && (inStr[strLen(inStr) - 1] == '\n')) {
         inStr[strLen(inStr) - 1] = '\0';
    }
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

        if (srcString[i] == ' ') {                      // if delimiter is found
            srcString[i] = 0x00;                    // replace with null char
            saved = 0;                              // marked as not saved (need to save on next iteration)
        } else if (saved == 0) {                        // if not saved
            tokens[numFound] = &srcString[i];       // save the address
            numFound++;
            saved = 1;                              // ignore (don't save anything)
        } else {                                        // all other characters
            saved = 1;                              // ignore (don't save anything)
        }
    }
    return numFound;
}
