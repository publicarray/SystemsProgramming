#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>
#include <stdio.h>

// http://en.cppreference.com/w/c/chrono/ctime
void localTime(void)
{
    time_t now = time(NULL);
    printf("%s", ctime(&now));

#ifdef __STDC_LIB_EXT1__
    char str[26];
    ctime_s(str, sizeof str, &now);
    printf("%s", str);
#endif
}


#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int path() {
   char cwd[3000];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       printf("%s\n", cwd);
   else
       perror("getcwd() error");
   return 0;
}
