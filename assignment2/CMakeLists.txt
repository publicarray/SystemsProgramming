cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ../bin)
project(SClientServer)

add_executable(main main.c lib.c)

install(TARGETS main DESTINATION bin)
