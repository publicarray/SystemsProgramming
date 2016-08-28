#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

void strInit(String* self) {
    self->bufferLength = 50;
    self->length = 0;
    self->data = (char *) malloc(sizeof(char *) * self->bufferLength);
}

void strCInit(String* self, char* init) {
    self->length = strlen(init);
    self->data = malloc(sizeof(char*) * self->length);
    strcpy(self->data, init);
}

int strLength(String* self) {
    // return strlen(self->data);
    return (int) self->length;
}

void strAddChar(String* self, char c) {
    strResize(self, self->length + 1);
    self->data[self->length] = c;
    self->data[++self->length] = '\0';
    // self->length[];
}

void strConcat(String* self, String* input) {
    int newLength = self->length + input->length;
    strResize(self, newLength);
    // for (int i = 0; i < strlen(input->data); i++) {
    for (int i = 0; i < input->length; i++) {
        self->data[self->length + i] = input->data[i];
    }
    self->length = newLength;
}

void strConcatCS(String* self, char* input) {
    strResize(self, strlen(input));
    for (int i = 0; i < strlen(input); i++) {
        self->data[self->length + i] = input[i];
    }
    self->length += strlen(input);
}

void strConcatI(String* self, int input) { // TODO: Refactor
    // printf("length: %d\n", floor(log10(abs(50))));
    char buffer[500];
    snprintf(buffer, 500, "%d", input);
    buffer[strlen(buffer)] = 0x00;
    // printf("length1:%i\n", (int) strlen(buffer));
    strConcatCS(self, buffer);
}

int strParseInt(String* self) {
    return atoi(self->data);
    // char * nums = "1234567890";
    // int number = 0;
    // number = sscanf(self->data,);
    // int negative = 0;
    // for (int i = 0; i < self->length; i++){
    //     if (self->data[i] == '-') {
    //         negative = 0;
    //     }
    // }
    // return number;
}

void strPrint(String* self) {
    printf("%s\n", self->data);
}

void strFree(String* self) {
    free(self->data);
    self->bufferLength = 0;
    self->length = 0;
}

void strResize(String* self, int newSize) {
    if (self->bufferLength <= self->length+newSize) {
        strIncBufSize(self, newSize);
    }
}

void strIncBufSize(String* self, int addSize) {
    addSize = (self->bufferLength < addSize) ? addSize : self->bufferLength;
    self->bufferLength = self->bufferLength + addSize;
    self->data = realloc(self->data, sizeof(char) * self->bufferLength); // TODO: error checking
}

// void splitString(String* self, char delemiter) {
//     int n = 0;
//     char *tokens[100];
//     token[n] = strtok(self->data, ", " );
//     while (token[n++]) {
//         tokens[n] = strtok(NULL, "," );
//     }
// }


// https://sucs.org/Knowledge/Help/Program%20Advisory/Reading%20an%20arbitrarily%20long%20line%20in%20C
int srtRead(String* self, FILE * fd) {
    size_t size = 0;
    size_t last = 0;

    while (!feof(fd) && self->data[last] != '\n') {
        size += BUFSIZ;
        strResize(self, size);

        if (fgets(self->data+last, size, fd) != NULL) {
            self->length = strlen(self->data);
            last = self->length - 1;
        } else {
            return 1;
        }
    }
    return 0;
}
