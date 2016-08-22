#include <stdio.h>
#include <string.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    char command[3000];
    while(1) {
        scanf("%s", command);
        if (strcmp(command, "calc") == 0) {
            char expression [2000];
            scanf("%s", expression);
            calulate(expression);
        }

        if (strcmp(command, "time") == 0) {
            localTime();
        }

        if (strcmp(command, "path") == 0) {
            path();
        }

        if (strcmp(command, "dump") == 0) {
            char pathname [2000];
            scanf("%s", pathname);
            dump(pathname);
        }

        if (strcmp(command, "list") == 0) {
            listdir(".");
        }
    }


    return 0;
}
