#define STDIN_FILENO 0
#define NB_ENABLE 1
#define NB_DISABLE 0

#ifdef CONSOLE_H
#define CONSOLE_H

int kbhit()
void nonblock(int state)
void BarDisplayEx(int full, int max, int *p1, int *p2, int blocked)
void BarDisplay(int full, int max)

#endif
