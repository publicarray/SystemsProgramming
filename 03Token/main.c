#include <stdio.h>

int strLength(char *string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

// int strLength(char *string) {
//     int length;
//     for (length = 0; string[length] != '\0'; length++);
//     return length;
// }

// http://fxr.watson.org/fxr/source/libkern/strlen.c?v=DFBSD
// int strLength(char *str) {
//     const char *s;
//         for (s = str; *s; ++s);
//         return(s - str);
// }

int ParseLine(char* srcString, char delimiter, char* tokens[], int maxTokens)
{
    int numFound = 0;
    int length = strLength(srcString);

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Hit max number of token limit, I need more space!\n");
            return numFound;
        }
        if (srcString[i] == delimiter) { // if delimiter is found
            tokens[numFound] = &srcString[i]; // save the address
            numFound++;
        }
    }
    return numFound;
}

int main(int argc, char const *argv[])
{
    int numTokensToSearchFor = 50;
    char string[] = "Hello World! My Name is Foo.";

    char* tokens[numTokensToSearchFor];
    int tokensFound = ParseLine(string, 'o', tokens, numTokensToSearchFor);

    printf("Number of tokens found: %d\n\n", tokensFound);

    for (int i = 0; i < tokensFound; i++) {
        printf("%s\n", tokens[i]);
    }
    return 0;
}
