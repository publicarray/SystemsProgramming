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

int ParseLine(char* srcString, char delimiter, const char* tokens[], int maxTokens)
{
    int numFound = 0;
    int size = strLength(srcString);
    // unsigned long size = sizeof(&srcString); // returns 8

    for (int i = 0; i < size; i++) {
        // printf("%d %d %ld\n", numFound, maxTokens, size);

        if (numFound >= maxTokens) {
            printf("Err: Hit max number of token limit, I need more space!\n");
            return numFound;
        }
        if (srcString[i] == delimiter) {
            tokens[numFound] = &srcString[i];
            numFound++;
        }
    }
    return numFound;
}

int main(int argc, char const *argv[])
{
    char string[] = "Hello World! My Name is Foo.";

    // printf("%ld\n", sizeof(string)); // returns 29

    const char* tokens[5];
    int tokensFound = ParseLine(string, 'o', tokens, 5);

    printf("Number of tokens found: %d\n\n", tokensFound);

    for (int i = 0; i < tokensFound; i++) {
        printf("%s\n", tokens[i]);
    }
    return 0;
}
