# Build & Install

## Makefile

```bash
make
make install
make uninstall

# or compile with mingw32: (Not Working Yet)
# make CC='i686-w64-mingw32-gcc -std=c99' OUT='main.exe'
```

## CMake

```bash
mkdir build
cd build
cmake ..
# or: cmake .. -G 'Xcode'
# or: cmake .. -G 'Visual Studio 12 2013'
make
make install
```

## Manually

### Compile and run the program

```bash
cc main.c lib.c Thread.c Mutex.c ConditionVariable.c Semaphore.c Job.c JobQueue.c -o bin/main && bin/main
```

# Usage

`main <number-of-threads>`

number-of-threads: default is 32 threads.

## Example

```bash
main 15
```

# Commands

\[32bit number\] - bit-shifts 32 times and factorises the result

0 - test mode

q, quit, exit - exits the program
