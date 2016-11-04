OUTDIR := bin/
ISTALLDIR := /usr/local/bin/
OUT := main

SRC = main.c Number.c

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

docs: ${SRC}
	doxygen

help:
	@echo "Usage: make <command>\n  all (default) - build the binary into bin/ using the source code. \n  help - displays usage information (this screen)\n  clean - removes build binary\n  install - copy binary to /usr/local/bin/\n  uninstall - remove binary from /usr/local/bin/\n  remove - alias to uninstall\n  docs - generate api documenation"
