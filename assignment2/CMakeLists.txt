cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ../bin)
project(assignment2)

add_executable(main main.c lib.c Thread.c Mutex.c ConditionVariable.c Semaphore.c Job.c JobQueue.c)

install(TARGETS main DESTINATION bin)
