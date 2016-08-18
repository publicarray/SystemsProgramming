# https://stackoverflow.com/questions/1484817/how-do-i-make-a-simple-makefile-for-gcc-on-linux
TARGET = prog
LIBS = -lm
CC = gcc-6
CFLAGS = -g -Wall
# -g for compiling debug information: http://www.rapidtables.com/code/linux/gcc/gcc-g.htm
# -Wall show all errors
# CFLAGS = -g0 -O3 -Wall
# http://www.rapidtables.com/code/linux/gcc/gcc-o.htm#optimization

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
