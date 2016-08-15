#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "List.h"

int main() {

    List array; initList(&array);
    float sample = 4.0;
    float* newData = (float*) addList(&array, &sample, sizeof (sample));
    printf("%f\n\n", *newData);

    float i;
    for(i = 0; i< 100;i++) {
        addList(&array, &i, sizeof (i));
        // printf("%f\n", *newData);
    }

    for(i = 0; i< 5;i++) {
        printf("%f\n", *((float*) getList(&array, i)));
    }
    // printf("%i\n", getList(&array), 3);


    // *getList(&array, 3) = 200;
    // SAME THING AS
    // int * ptr = getList(&array, 3);
    // ptr = 200;

    freeList(&array);


    String str1, str2, str3;

    strInit(&str1);
    strCInit(&str2, "Hello Foo Bar");
    strCInit(&str3, "201");
    printf("str2 length: %i\n", strLength(&str2));
    strPrint(&str1);
    strPrint(&str2);
    printf("str3 length: %i\n", strLength(&str3));
    strAddChar(&str3, '6');
    printf("str3 length: %i\n", strLength(&str3));
    printf("str3 integer: %i\n", strParseInt(&str3));

    printf("\n\n");
    strPrint(&str2);
    printf("str2 length: %i\n", strLength(&str2));
    strConcat(&str2, &str3);
    strPrint(&str2);
    printf("str2 length: %i\n", strLength(&str2));
    strConcatCS(&str2, " Baz");
    strPrint(&str2);
    printf("str2 length: %i\n", strLength(&str2));
    int value = 810;
    strConcatI(&str2, value);
    strPrint(&str2);
    printf("str2 length: %i\n", strLength(&str2));
    return 0;
}
