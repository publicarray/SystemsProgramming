#include <sys/time.h>
#include <termios.h>
#include <stdio.h>
#include "console.h"

//----------------------------------
//--- check for keypress     -------
//----------------------------------
int kbhit()
{
    struct timeval tv;
    fd_set fds;

    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}


//----------------------------------
//--- allow nonblocking reading ----
//----------------------------------
void nonblock(int state)
{
    struct termios ttystate;

    tcgetattr(STDIN_FILENO, &ttystate); //- get the terminal state

    if (state == NB_ENABLE) {
        ttystate.c_lflag &= ~ICANON;//- turn off canonical mode
        ttystate.c_cc[VMIN] = 1; //- minimum input chars read
    } else if (state == NB_DISABLE) {
        ttystate.c_lflag |= ICANON; //- turn on canonical mode
    }
    //- set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}


//----------------------------------
//--- display output bar     -------
//----------------------------------

void BarDisplayEx(int full, int max, int *p1, int *p2, int blocked)
{
    int i;

    for (i = 0; i < max; i++) {
        printf("#");
    }
    if (p2) {
        printf("     P:%2d C:%2d  ", *p1, *p2);
        if (blocked) {
            printf("BLOCKED");
        }else {
            printf("       ");
        }
        for (i = 0; i < max + 16; i++) {
            printf("\b");
        }
    }
    for (i = 0; i < max; i++) {
        printf("\b");
    }
    for (i = 0; i < full; i++) {
        printf("_");
    }
    for (i = 0; i < full; i++) {
        printf("\b");
    }
    fflush(0);
}


void BarDisplay(int full, int max)
{
    BarDisplayEx(full, max, 0, 0, 0);
}
