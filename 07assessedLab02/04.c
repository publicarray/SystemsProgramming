#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int strLength(char *string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

int splitStr(char* srcString, char* tokens[], int maxTokens)
{
    int numFound = 0;
    int saved = 0;
    int length = strLength(srcString);

    for (int i = 0; i < length; i++) { // search each char
        if (numFound >= maxTokens) {
            printf("Err: Hit max number of token limit, I need more space!\n");
            return numFound;
        }

        if (srcString[i] == ' ') { // if delimiter is found
            srcString[i] = 0x00; // replace with null char
            saved = 0; // marked as not saved (need to save on next iteration)
        } else if (saved == 0) { // if not saved
            tokens[numFound] = &srcString[i]; // save the address
            numFound++;
            saved = 1; // ignore (don't save anything)
        } else { // all other characters
            saved = 1; // ignore (don't save anything)
        }
    }
    return numFound;
}

void byebye() {
    puts("bye bye");
}

int execute(char* cmd) {
    atexit(byebye); // register exit function

    char str[400]; // TODO need to fix this
    strcpy(str, cmd);

    int maxTokens = 100;
    char* tokens[100];
    int found = splitStr(str, tokens, maxTokens);
    tokens[found] = 0x0;
    int status;

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // child
        int out = execvp(tokens[0], tokens);
        if (out < 0) {
            perror("exec");
        }
        exit(out);
    } else { // parent
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

int main(int argc, char const *argv[]) {

    if (argc == 1) {
        FILE *fin; // input
        int BSIZE = 2000;
        char line[BSIZE]; // buffer
        int c;
        int i = 0;

        fin = fopen(argv[1], "r");

        while ((c = fgetc(fin)) > 0) {
            if (!feof(fin)) {
                if (c == '\r') {
                    printf("> %s\n", line);
                    execute(line);
                    i = 0;
                    memset(line, 0x0, BSIZE);
                } else if (c == '\n') {
                    printf("> %s\n", line);
                    execute(line);
                    i = 0;
                    memset(line, 0x0, BSIZE);
                } else {
                    line[i] = c;
                    i++;
                }
            }
        }
        fclose(fin);
        return 0;
    } else {
        puts("usage: programName [fileName]\n");
        return 1;
    }
}
