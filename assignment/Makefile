OUTDIR := bin/
ISTALLDIR := /usr/local/bin/
SERVER := server
CLIENT := client

SERVER_SRC = server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c server/uname.c server/cp.c server/wait.c
CLIENT_SRC = client/main.c ../SocketType.c lib.c

CFLAGS := -Wall -O3
CC := gcc
# CC := i686-w64-mingw32-gcc -std=c99  # windows

.PHONY: all ${OUTDIR}${SERVER} ${OUTDIR}${CLIENT} clean install uninstall remove

all: ${OUTDIR}${SERVER} ${OUTDIR}${CLIENT}

${OUTDIR}${SERVER}: ${SERVER_SRC}
	${CC} ${CFLAGS} $? -o ${OUTDIR}${SERVER}

${OUTDIR}${CLIENT}: ${CLIENT_SRC}
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
