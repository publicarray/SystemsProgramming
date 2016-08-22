#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int maths (int result, int a, char x) {
        if (x == '+') {
            result += a;
        } else if (x == '-') {
            result -= a;
        } else if (x == '*') {
            result *= a;
        } else if (x == '/') {
            result /= a;
        }
        return result;
}
void calulate (char* str) {

    int num = 0;
    int result = 0;
    for (int i = 0; strlen(str) > i; i++) {
        if (isdigit(str[i])) {
            num = atoi((&str[i]));
        } else {
           result = maths(result, num, str[i]);
        }

        if (strlen(str) -2 == i) {
            result = maths(result, atoi(&str[i+1]), str[i]);
        }
    }

    printf("%d\n", result);
}
