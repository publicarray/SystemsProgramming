#include <stdio.h>
#include <ctype.h>


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
