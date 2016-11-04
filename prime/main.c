#include <stdio.h>
#include <stdlib.h>

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
    x = newNumber("0");
    y = newNumber("1927");
    x.print(&x);
    y.print(&y);
    sum = x.add(&x, &y);
    sum.print(&sum);

    x.free(&x);
    y.free(&y);
    sum.free(&sum);
    return 0;
}
