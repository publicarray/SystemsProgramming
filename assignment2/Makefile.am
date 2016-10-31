AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
CFLAGS = --pedantic -Wall -std=c99 -O3
LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = main
main_SOURCES = main.c lib.c Thread.c Mutex.c ConditionVariable.c Semaphore.c  Request.c Job.c JobQueue.c
