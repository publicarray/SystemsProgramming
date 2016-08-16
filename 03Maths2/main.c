#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define ARRLEN(x) (sizeof(x) / sizeof((x)[0]))
// #define ARRLEN(x) (sizeof (x) / sizeof(*x))


double multiply(double x) {
    return x*x;
}

double multiply2(double x) {
    return x*x*x;
}

double magicFunc(double x) {
    return 2*x*x-3*x;
}
double delimiter(double x) {
    return 1/x;
}

double oneDevideXX(double x) {
    return 1/(x*x);
}

void generate (double (*doSomething)(double), double *nums, int items) {
    printf("%8s | %-8s\n-------------------\n", "x", "y");
    double sum = 0;
    double awnsers[items];
    for (int i = 0; i < items; i++) {
        awnsers[i] = doSomething(nums[i]);
        sum += awnsers[i];
        printf("%8lg | %-8lg\n", nums[i], awnsers[i]);
    }

    printf("\n");
    for (int i = 0; i < items; i++) {
        if (i < items-1) {
            printf("%lg+", awnsers[i]);
        } else {
            printf("%lg = %lg\n", awnsers[i], sum);
        }
    }
}

int main(int argc, char const *argv[]) {
    typedef double (* Func) (double);

    int i;
    int option;
    int items = argc -1;
    double nums[argc];
    Func mathsFuncs[] = {sin, cos, tan, log, sqrt, multiply, multiply2, magicFunc, delimiter, oneDevideXX};

    if (argc <= 1) {printf("usage: %s <args>\nPlease enter the values of x separated by spaces\n", argv[0]); return 1;}

    double temp = 0;
    for (i = 0; i < items; i++) {
        sscanf(argv[i+1], "%lf", &temp); // convert to double
        nums[i] = temp;
    }

    printf("Please select one of the following functions:\n");
    printf("1 sin(x):\n");
    printf("2 cos(x):\n");
    printf("3 tan(x):\n");
    printf("4 log(x):\n");
    printf("5 square root(x):\n");
    printf("6 x*x:\n");
    printf("7 x*x*x:\n");
    printf("8 2*x*x-3*x:\n");
    printf("9 1/x:\n");
    printf("10 1/(x*x):\n");
    scanf("%i", &option);

    if (option > ARRLEN(mathsFuncs)) {printf("Error: Incorrect Option\n"); return 1;}
    generate(mathsFuncs[option-1], nums, items);
    return 0;
}
