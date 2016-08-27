#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "String.h"
#include "common.h"
#include "../lib.h"

int main(int argc, char const *argv[])
{

    char inStr[3000];
    // char* command = malloc(3000);
    // if (command == NULL) {
    //     printf ("Failed to allocate memory\n");
    //     return 1;
    // }

    int numTokensToSearchFor = 50;
    char* arguments[numTokensToSearchFor];
    int argCount = 0;

    while(1) {
        fgets(inStr, sizeof inStr, stdin);

        /* Remove trailing newline, if it's there. */
        if ((strlen(inStr) > 0) && (inStr[strlen (inStr) - 1] == '\n')) {
             inStr[strlen (inStr) - 1] = '\0';
        }
        puts(inStr);
        // scanf("%s", command);
        // if (strcmp(command, "calc") == 0) {
        //     char expression [2000];
        //     scanf("%s", expression);
        //     calulate(expression);
        // }

        // if (strcmp(command, "time") == 0) {
        //     localTime();
        // }

        // if (strcmp(command, "path") == 0) {
        //     path();
        // }

        argCount = splitStr(inStr, arguments, numTokensToSearchFor);
        // printf("Number of arguments found: %d\n\n", argCount);
        // for (int i = 0; i < argCount; i++) {
        //     printf("%s\n", arguments[i]);
        // }

        if (strcmp(arguments[0], "dump") == 0 && argCount > 1) {
            dump(arguments[1]);
        }

        if (strcmp(arguments[0], "list") == 0) {
             if (argCount == 1) {
                listdir("."); // if no arguments use current directory
            } else if (argCount > 1) {
                listdir(arguments[1]);
            }
        }
    }

    // free (command);
    return 0;
}
