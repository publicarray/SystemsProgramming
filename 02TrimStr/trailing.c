#include <strings.h>
// #include "include.h"

//-- Removes trailing blanks from s --
void trailing(char *s) {
    // int index = MAXLEN;
    int slength = strlen(s);

    while ((s[slength] == ' ' || s[slength] == '\t' || s[slength] == '\0')) {
        slength--;
    }

    s[slength] = '\0';
}
