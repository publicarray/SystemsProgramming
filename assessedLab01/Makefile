OUT := shell
SOURCE := $(wildcard *.c)
OBJECTS := $(SOURCE:.c=.o)
CC := gcc
# CC := i686-w64-mingw32-gcc -std=c99  # windows
CFLAGS := -Wall -O3

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUT)

clean:
	$(RM) $(OBJECTS)
