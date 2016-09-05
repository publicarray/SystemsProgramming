make clean
make
./shell

make clean
make CC=i686-w64-mingw32-gcc CFLAGS=-std=c99 OUT=shell.exe
wine ./shell.exe

