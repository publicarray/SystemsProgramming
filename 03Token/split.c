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
    int numFound = -1;
    int length = strLength(srcString);

    tokens[0] = &srcString[0];
    numFound++;

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Hit max number of token limit, I need more space!\n");
            return numFound;
        }
        if (srcString[i] == ' ') { // if delimiter is found
            srcString[i] = 0x00; // replace with null char
            tokens[numFound+1] = &srcString[i+1]; // save the address
            numFound++;
        }
    }
    return numFound;
}


int main(int argc, char const *argv[])
{
    int numTokensToSearchFor = 50;
    char string[] = "Hello World!    My Name is Foo.";

    char* tokens[numTokensToSearchFor];
    int tokensFound = ParseLine(string, tokens, numTokensToSearchFor);

    printf("Number of tokens found: %d\n\n", tokensFound);

    for (int i = 0; i < tokensFound; i++) {
        printf("%s\n", tokens[i]);
    }
    return 0;
}
