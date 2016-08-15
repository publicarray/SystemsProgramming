#include <strings.h>
//-- Removes trailing blanks from s --
void trailing(char *s)
{
    // int index = MAXLEN;
    int slength = strlen(s);
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == ' ' || s[i] == '\t') {
            slength--;
        }
    }
    // while ((s[slength] == ' ' || s[slength] == '\t')) {
    //     --slength;
    // }

    s[slength] = '\0';
}
