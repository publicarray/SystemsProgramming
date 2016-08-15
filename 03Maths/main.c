#include <stdio.h>
#include <math.h>
#define ARRLEN(x) (sizeof(x) / sizeof((x)[0]))

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
    // if (x == 0) {return INFINITY;}
    return 1/x;
}

double oneDevideXX(double x) {
    // if (x == 0) {return INFINITY;}
    return 1/(x*x);
}

void generate (double (*doSomething)(double), double xStart, double xFinal, double xStepSize) {
    printf("%8s | %-8s\n-------------------\n", "x", "y");
    for(double i = xStart; i <= xFinal; i+=xStepSize) {
        printf("%8lg | %-8lg\n", i, doSomething(i));
    }
}

int main() {
    typedef double (* Func) (double);
    Func mathsFuncs[] = {sin, cos, tan, log, sqrt, multiply, multiply2, magicFunc, delimiter, oneDevideXX};

    double xStart, xFinal, xStepSize;
    int option;

    // %lf = double
    // %lu = unsigned long
    printf("Please enter the starting value of x: ___\n");
    scanf("%lf", &xStart);

    printf("Please enter the final value of x: ___\n");
    scanf("%lf", &xFinal);

    printf("Please enter the step size of x: ___\n");
    scanf("%lf", &xStepSize);

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

    if (option > ARRLEN(mathsFuncs)) {printf("Error: Incorrect Option\n");return 1;}
    generate(mathsFuncs[--option], xStart, xFinal, xStepSize);

    return 1;
}

