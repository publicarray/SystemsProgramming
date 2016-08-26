#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "String.h"
#include "common.h"

int main(int argc, char const *argv[])
{

    // char command[3000];
    char* command = malloc(3000);
    if (command == NULL) {
        printf ("Failed to allocate memory\n");
        return 1;
    }

    while(1) {
        fgets(command, sizeof command, stdin);

        /* Remove trailing newline, if it's there. */
        if ((strlen(command) > 0) && (command[strlen (command) - 1] == '\n')) {
             command[strlen (command) - 1] = '\0';
        }

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

        if (strcmp(command, "dump") == 0) {
            char pathname [2000];
            scanf("%s", pathname);
            dump(pathname);
        }

        if (strcmp(command, "list") == 0) {
            char pathname [2000] = ".";
            // scanf("%s", pathname);
            listdir(pathname);
        }
    }

    free (command);
    return 0;
}
