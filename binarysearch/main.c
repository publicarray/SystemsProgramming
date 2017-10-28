#include <stdio.h>
#include <math.h>

// Choose a number from 1 to 100(maxNum)
// the aim is to guess the correct number in 7 guesses
// I will tell you if you are too high or to low

int answer = 55; // Your chosen number
int maxNum = 100;
int verbose = 1;

int guessTheAnswer(int guess);
void comparisonLoopSearch ();
void binarySearch ();
int mylog2(int num);

int main (int argc, char const *argv[]) {
    // ToDo: get awnser and maxNum from commandline arguments
    comparisonLoopSearch(maxNum);
    binarySearch(maxNum);
    return 0;
}

// -1 tooHigh
// 0 correct answer
// 1 tooLow
int guessTheAnswer(int guess) {
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
