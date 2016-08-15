#include <stdio.h>
#include "include.h"

int main()
{
    char instring[MAXLEN];
    printf("Enter a string with leading and trailing blanks:\n");
    gets(instring);
    printf("input = '%s'\n", instring);
    leading(instring);          //- Remove leading blanks
    trailing(instring);             //- Remove trailing blanks
    printf("output = '%s'\n", instring);    //- Print modified string
    return 0;
}
