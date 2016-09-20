#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

void strInit(String* self) {
    self->bufferLength = BUFSIZ; // BUFSIZ size of the buffer used by setbuf, may be an optimised size for the OS
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
    if (self == NULL) {return;}

    strResize(self, 1);
    self->data[self->length] = c;
    self->data[++self->length] = '\0';
    // self->length[];
}
void strConcatC(String* self, char c) {
    strAddChar(self, c);
}

void strConcat(String* self, String* input) {
    if (self == NULL) {return;}

    int newLength = self->length + input->length;
    strResize(self, newLength);
    // for (int i = 0; i < strlen(input->data); i++) {
    for (int i = 0; i < input->length; i++) {
        self->data[self->length + i] = input->data[i];
    }
    self->length = newLength;
}

void strConcatCS(String* self, char* input) {
    if (self == NULL) {return;}

    strResize(self, strlen(input));
    for (int i = 0; i < strlen(input); i++) {
        self->data[self->length + i] = input[i];
    }
    self->length += strlen(input);
    // printf("self-> length:%i bufferlength:%i\n", self->length, self->bufferLength);
}

void strOverrideCS(String* self, char* input) {
    if (self == NULL) {return;}

    self->length = 0;
    strConcatCS(self, input);
    self->data[self->length] = 0x00;
}

void strConcatI(String* self, int input) { // TODO: Refactor
    if (self == NULL) {return;}

    // printf("length: %d\n", floor(log10(abs(50))));
    char buffer[500];
    snprintf(buffer, 500, "%d", input);
    buffer[strlen(buffer)] = 0x00;
    // printf("length1:%i\n", (int) strlen(buffer));
    strConcatCS(self, buffer);
}

void strConcatF(String* self, float input) { // TODO: Refactor
    if (self == NULL) {return;}

    char buffer[500];
    snprintf(buffer, 500, "%.10f", input);
    buffer[strlen(buffer)] = 0x00;
    strConcatCS(self, buffer);
}

int strParseInt(String* self) {
    if (self == NULL) {return 0;}

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
    if (self == NULL) {return;}

    printf("%s\n", self->data);
}

void strClean(String* self) {
    if (self == NULL) {return;}

    memset(self->data, 0x00, self->length);
    self->length = 0;
}

void strFree(String* self) {
    if (self == NULL) {return;}

    free(self->data);
    self->bufferLength = 0;
    self->length = 0;
}

void strResize(String* self, int addSize) {
    // printf("rezize?  bufferlength: %i <= length+newSize: %i\n", self->bufferLength, self->length+newSize);
    if (self->bufferLength <= self->length+addSize) {
        addSize = (BUFSIZ < addSize) ? addSize : BUFSIZ; // doubling the buffer is generally better unless when addSize is bigger than the buffer
        self->bufferLength = self->bufferLength + addSize; // increase the size
        self->data = realloc(self->data, sizeof(char) * self->bufferLength); // TODO: error checking
    }
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
