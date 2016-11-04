#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Number.h"

int main(int argc, char const *argv[])
{
    Number x; x = newNumber("1245");
    Number y; y = newNumber("3541");
    x.print(&x);
    y.print(&y);
    Number sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    //////////
    puts("");
    x = newNumber("2455");
    y = newNumber("1927");
    x.print(&x);
    y.print(&y);
    sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    //////////
    puts("");
    x = newNumber("1927");
    y = newNumber("1");
    x.print(&x);
    y.print(&y);
    sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    //////////
    puts("");
    x = newNumber("1927");
    y = newNumber("-20"); // TODO
    x.print(&x);
    y.print(&y);
    sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    //////////
    puts("");
    unsigned long long int max = ULLONG_MAX;
    char * str = malloc(500000);
    sprintf(str, "%llu", max);
    printf("LLONG_MAX: %llu\n", max);
    printf("LLONG_MAX+2: %llu\n", max+2); // do demonstrate that it really is the largest number
    x = newNumber(str);
    y = newNumber(str);
    x.print(&x);
    y.print(&y);
    sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    free(str);
    return 0;
}
