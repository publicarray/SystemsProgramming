#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>
#include <stdio.h>

// http://en.cppreference.com/w/c/chrono/ctime
int main(void)
{
    time_t now = time(NULL);
    printf("%s", ctime(&now));

#ifdef __STDC_LIB_EXT1__
    char str[26];
    ctime_s(str, sizeof str, &now);
    printf("%s", str);
#endif
}
