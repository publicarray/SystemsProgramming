OUTDIR := bin/
ISTALLDIR := /usr/local/bin/
OUT := main

SRC = main.c lib.c Thread.c Mutex.c ConditionVariable.c Semaphore.c  Request.c Job.c JobQueue.c

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
	doxygen doc.cfg

help:
	@echo "Usage: make <command>\n  all (default) - build the binary into bin/ using the source code. \n  help - displays usage information (this screen)\n  clean - removes build binary\n  install - copy binary to /usr/local/bin/\n  uninstall - remove binary from /usr/local/bin/\n  remove - alias to uninstall\n  docs - generate api documenation"
