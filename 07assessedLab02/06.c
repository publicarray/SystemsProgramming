#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int execute(char* cmd) {
    int status;
    char *number;
    int num;

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // child
        printf("pid=%i\n", getpid());
        number = getenv("S_NUMBER");
        num = atoi(number);
        printf("child: S_NUMBER=%i\n", num);
        exit(num);
    } else { // parent
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        char buf[300] = "";
        puts(argv[1]);
        int result = atoi(argv[1]) * 2;
        sprintf(buf, "S_NUMBER=%d", result);
        putenv(buf);
        printf("return value: %i\n", execute(argv[1]));
        return 0;
    } else {
        puts("usage: programName [number]\n");
        return -1;
    }
}
