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

int strLength(String* self);
void strAddChar(String* self, char c);
void strConcatC(String* self, char c);
void strConcat(String* self, String* input);
void strConcatCS(String* self, char* input);
void strOverrideCS(String* self, char* input);
void strConcatI(String* self, int input);
void strConcatF(String* self, float input);

void strPrint(String*  self);
int strParseInt(String* self);
// char* split(String* self, char delimiter){};
void strClean(String* self);
void strFree(String* self);
void strResize(String* self, int newSize);

// charAt (String* self, int num)
void strIncBufSize(String* self, int addSize);
int srtRead(String* self, FILE * fd);

#endif
