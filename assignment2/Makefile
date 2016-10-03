OUTDIR := bin/
ISTALLDIR := /usr/local/bin/
OUT := main

SRC = main.c lib.c
# ../01StringAndList/String.c
#../SocketType.c
CFLAGS := -Wall -O3
CC := gcc
# CC := i686-w64-mingw32-gcc -std=c99  # windows

.PHONY: all ${OUTDIR}${OUT} clean install uninstall remove

all: ${OUTDIR}${OUT}

${OUTDIR}${OUT}: ${SRC}
	${CC} ${CFLAGS} $? -o ${OUTDIR}${OUT}

clean:
	rm -f ${OUTDIR}${OUT}

install:
	cp ${OUTDIR}${OUT} ${ISTALLDIR}

uninstall:
	rm -f ${ISTALLDIR}$(OUT)

remove: uninstall
