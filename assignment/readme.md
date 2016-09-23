# Build & Install

## Makefile

```sh
make
make install

# or compile with mingw32:
make CC='i686-w64-mingw32-gcc -std=c99' SERVER='server.exe' CLIENT='client.exe'
```

## CMake

```sh
mkdir build
cd build
cmake ..
# or: cmake .. -G 'Xcode'
# or: cmake .. -G 'Visual Studio 12 2013'
make
make install
```

## Manualy

### Compile and run the server

```bash
cc server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c server/uname.c server/cp.c server/sleep.c -o bin/server && bin/server -p5001
```

### Compile and run the client

```bash
cc client/main.c ../SocketType.c lib.c ../01StringAndList/String.c -o bin/client && bin/client -p 5001 localhost
```

# Usage

## Example
```sh
server -p 5001
client -p 5001 localhost
```

## Server

server -p - Port number to bind the server to

## Client

client -p [IP address or host name] - Port number to listen to

# Commands

list [-l] [-f] [pathname] [localfile] - lists the files in the current or given directory to the screen or prints them to a file. Options: -l = long list, -f force overwrite

get filepath [-f] [localfile] - prints the content of a nominated file to screen or to the given file

put localfile [-f] [fileName] - creates a remote copy of a local file with same or user specified fileName

sys - returns the name /version of the Operating System and CPU type

delay [integer] - returns the given number after a delay of seconds.

quit - exits the program

exit - exits the program
