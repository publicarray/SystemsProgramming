#include <stdio.h>
#include <errno.h>
#include "../../01StringAndList/String.h"
#include <unistd.h>

int delay (int seconds, String *out) {
    sleep(seconds);

    if (out == NULL) {
        printf("delayed: %d\n", seconds);
    } else {
        strConcatCS(out, "delayed: ");
        strConcatI(out, seconds);
    }
    return seconds;
}
