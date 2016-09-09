#include <stdio.h>

int strLength(char *string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

int ParseLine(char* srcString, char* tokens[], int maxTokens)
{
    int numFound = 0;
    int saved = 0;
    int length = strLength(srcString);

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Hit max number of token limit, I need more space!\n");
            return numFound;
        }

        if (srcString[i] == ' ') { // if delimiter is found
            srcString[i] = 0x0; // replace with null char
            saved = 0; // marked as not saved (need to save on next iteration)
        } else if (saved == 0) { // if not saved
            tokens[numFound] = &srcString[i]; // save the address
            numFound++;
            saved = 1; // ignore (don't save anything)
        } else { // all other characters
            saved = 1; // ignore (don't save anything)
        }
    }
    tokens[numFound] = 0x0; // insert null at the end of the array
    return numFound;
}

int main(int argc, char const *argv[])
{
    int numTokensToSearchFor = 50;
    char string[] = "Hello World!        My Name is Foo.";

    char* tokens[numTokensToSearchFor];
    int tokensFound = ParseLine(string, tokens, numTokensToSearchFor);

    printf("Number of tokens found: %d\n\n", tokensFound);

    for (int i = 0; i < tokensFound; i++) {
        printf("%s\n", tokens[i]);
    }
    return 0;
}
