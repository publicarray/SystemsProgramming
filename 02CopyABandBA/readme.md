# debug

```
gcc main.c && ./a.out
cargo run
go run -gcflags "-N" main.go
```

# release

```
gcc main.c -O1 ./a.out
cargo run --release
go run -ldflags "-s -w" main.go
```

**Note**: gcc -O2 will probably optimise the loops away!

clock_t measures raw processor clock time since the program is started. It represents the process running time i.e. CPU time is the total execution time or runtime for which the CPU was dedicated to a process. 

in golang's example the time refers to the total time spend in terms of human time. Any CPU interrupt and process switching is included.

see: https://service.futurequest.net/index.php?/Knowledgebase/Article/View/407
