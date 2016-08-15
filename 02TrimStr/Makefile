OUT := stripws
SOURCE := $(wildcard *.c)
OBJECTS := $(SOURCE:.c=.o)

CC := gcc-6
# debugging
CFLAGS := -Wall -g
# production
# CFLAGS := -Wall -O3

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUT)

clean:
	$(RM) $(OBJECTS)
	$(RM) -dfr $(OUT).dSYM
