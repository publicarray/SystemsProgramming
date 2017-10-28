#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Choose a number from 1 to 100(maxNum)
// the aim is to guess the correct number in 7 guesses
// I will tell you if you are too high or to low

int answer = 55; // Your chosen number
int maxNum = 100;
int verbose = 1;

int guessTheAnswer (int guess);
void comparisonLoopSearch ();
void binarySearch ();
int isStrANumber (const char * str);

int isStrANumber (const char * str) {
    int length = strlen (str);
    for(int i = 0; i < length; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main (int argc, char const *argv[]) {

    if (argc > 1) {
        if (!isStrANumber(argv[1])) {
            printf ("Error: '%s' is not a number.\n", argv[1]);
            exit(1);
        }
        answer = atoi(argv[1]);
    }
    if (argc > 2) {
        if (!isStrANumber(argv[2])) {
            printf ("Error: '%s' is not a number.\n", argv[2]);
            exit(1);
        }
        maxNum = atoi(argv[2]);
    }

    comparisonLoopSearch(maxNum);
    binarySearch(maxNum);
    return 0;
}

// -1 tooHigh
// 0 correct answer
// 1 tooLow
int guessTheAnswer (int guess) {
    if (answer < guess) {
        return -1;
    } else if (answer > guess) {
        return 1;
    } else {
        return 0;
    }
}

void comparisonLoopSearch () {
    for (int guess = 0; guess <= maxNum; guess++ ) {
        if (guessTheAnswer(guess) == 0) {
            printf("comparisonLoopSearch - found answer: %d in %d guesses\n", guess, guess);
        }
    }
}

void binarySearch () {
    int guesses = 0;
    int midPoint = maxNum/2;
    int upperBound = maxNum+1;
    int lowerBound = 0;

    int maxGuesses = log2(maxNum);
    printf("binarySearch - maxGuesses: %d\n", maxGuesses);
    while (guesses <= maxGuesses) { // Logorithmic time -> O log (n)
        if (verbose == 1) {
            printf("binarySearch - guess: %d\n", midPoint);
        }

        guesses++;
        int test = guessTheAnswer(midPoint);
        if (test == -1) { // -1 tooHigh
            upperBound = midPoint;
        } else if (test == 1 ) { // 1 tooLow
            lowerBound = midPoint;
        } else {
            printf("binarySearch - found answer: %d in %d guesses\n", midPoint, guesses);
            return;
        }
        midPoint = lowerBound + (upperBound - lowerBound) / 2;
    }
}
