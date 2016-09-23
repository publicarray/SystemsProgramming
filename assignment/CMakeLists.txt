cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ../bin)
project(SClientServer)

add_executable(server server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c server/uname.c server/cp.c server/sleep.c)
add_executable(client client/main.c ../SocketType.c lib.c ../01StringAndList/String.c)

install(TARGETS server DESTINATION bin)
install(TARGETS client DESTINATION bin)
