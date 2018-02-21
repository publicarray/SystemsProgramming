#ifndef STRING_H
#define STRING_H

#include <stdio.h>

typedef struct String_Struct {
    unsigned int bufferLength;
    unsigned int length;
    char* data; // heap
} String;

void strInit(String* self);
void strCInit(String* self, char* init);

unsigned int strLength(String* self);
void strAddChar(String* self, char c);
void strConcatC(String* self, char c);
void strConcat(String* self, String* input);
void strConcatCS(String* self, char* input);
void strOverrideCS(String* self, char* input);
void strConcatI(String* self, int input);
void strConcatF(String* self, float input);

void strPrint(String*  self);
int strParseInt(String* self);
void strClean(String* self);
void strFree(String* self);
void strResize(String* self, int newSize);

// int srtRead(String* self, FILE * fd);

#endif
