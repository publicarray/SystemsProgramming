#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "String.h"
#include "common.h"
#include "../lib.h"

int main(int argc, char const *argv[])
{

    opterr = 0; // disable getopt's own error messages e.g. case '?'
    int i, c, fflag, lflag, argCount, maxArguments = 10;
    char inStr[3000];
    char* arguments[maxArguments];

    while(1) {
        fflag = 0; // reset flags
        lflag = 0;
        argCount = 0;

        fgets(inStr, sizeof inStr, stdin);

        // Remove trailing newline
        if ((strlen(inStr) > 0) && (inStr[strlen (inStr) - 1] == '\n')) {
             inStr[strlen (inStr) - 1] = '\0';
        }

        argCount = splitStr(inStr, arguments, maxArguments);
        // printf("Number of arguments found: %d\n\n", argCount);
        // for (i = 0; i < argCount; i++) {
        //     printf("%s\n", arguments[i]);
        // }
        while ((c = getopt(argCount, arguments, "fl")) != EOF) {
            switch (c) {
                case 'f':
                    fflag = 1;
                    break;
                case 'l':
                    lflag = 1;
                    break;
                case '?':
                    fprintf(stderr, "invalid option: -%c\n", optopt);
            }
        }

        if (fflag) printf("Option f is set\n");
        if (lflag) printf("Option l is set\n");

        // argv is reordered with the non-option arguments at the end
        // optind contains the index to the next argv argument for a subsequent call to getopt()
        //
        // skip arguments that have been processed
        // arguments += optind;
        argCount -= optind;
        // printf("Number of arguments found: %d\n\n", argCount);
        // printf("Number of arguments found: %d\n\n", optind);
        // for (i = 0; i < 10; i++) {
        //     printf("%i: %s\n", i, arguments[i]);
        // }

        // if (arguments) {
        //     printf("\nIgnored arguments:\n");
        //     while (optind--) {
        //         printf("    arg: %s\n", arguments);
        //     }
        // }
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


        if (argCount > 0 && strcmp(arguments[0], "dump") == 0) {
            dump(arguments[1]);
        }

        if (arguments[0] && strcmp(arguments[0], "list") == 0) {
             if (argCount == 0) {
                listdir("."); // if no arguments use current directory
            } else if (argCount > 0) {
                listdir(arguments[optind]);
            }
        }

        // reset getopt
        optreset = 1;
        optind = 1;
    }

    return 0;
}
