#include <stdio.h>
#include <errno.h>
#include "../../01StringAndList/String.h"
#include <sys/utsname.h>

int sys(String* out)
{
    char buf[500];
    struct utsname sysInfo;

    int returnCode = uname(&sysInfo);
    if (returnCode == -1) {
        sprintf(buf, "uname() error'd");
        perror("uname");
        return errno;
    }

    sprintf(buf, "OS: %s %s \nBuild: %s\nCPU: %s\nHostname: %s\n", sysInfo.sysname, sysInfo.release, sysInfo.version, sysInfo.machine, sysInfo.nodename);
    if (out == NULL) {
        printf("%s\n", buf);
    } else {
        strConcatCS(out, buf);
    }
    return 0;
}
