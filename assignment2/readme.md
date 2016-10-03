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

### Compile and run the program

```bash
cc main.c lib.c -o bin/main && bin/main
```

# Usage

## Example

main

```sh
main
```


# Commands

[number] 

0 - test mode

q, quit, exit - exits the program
