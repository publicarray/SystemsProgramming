OUTDIR := bin/
ISTALLDIR := /usr/local/bin/
SERVER := server
CLIENT := client

SSRC = server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c
CSRC = client/main.c ../SocketType.c lib.c

CFLAGS := -Wall -O3
CC := gcc
# CC := i686-w64-mingw32-gcc -std=c99  # windows

.PHONY: all clean ${OUTS} ${OUTC} install

all: ${OUTDIR}${SERVER} ${OUTDIR}${CLIENT}

${OUTDIR}${SERVER}: ${SSRC}
	${CC} ${CFLAGS} $? -o ${OUTDIR}${SERVER}

${OUTDIR}${CLIENT}: ${CSRC}
	${CC} ${CFLAGS} $? -o ${OUTDIR}${CLIENT}

clean:
	rm -f ${OUTDIR}${SERVER}
	rm -f ${OUTDIR}${CLIENT}

install:
	cp ${OUTDIR}${SERVER} ${ISTALLDIR}
	cp ${OUTDIR}${CLIENT} ${ISTALLDIR}

uninstall:
	rm -f ${ISTALLDIR}$(SERVER)
	rm -f ${ISTALLDIR}$(CLIENT)

remove: uninstall
