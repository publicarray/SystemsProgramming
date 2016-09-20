#ifndef LIB_H
#define LIB_H

#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

int strLen(char *string);
void removeNewLine (char* inStr);
void strToLower(char *string);
int splitStr(char* srcString, char* tokens[], int maxTokens);
struct timespec getTime();
float getTimeLapsed(struct timespec start);
float getTimeNsec();

#endif
