#include "include.h"

void leading(char *s)
{
    int n = 0;
    while (s[n] == ' ' || s[n] == '\t') {
        ++n;
    }
    if (n > 0) {
        adjustr(s, n);
    }
}
