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
