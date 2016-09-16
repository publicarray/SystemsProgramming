
## Compile and run the server

```bash
cd assignment
cc server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c -o bin/server && bin/server -p5001
````

## Compile and run the client
```bash
cd assignment
cc client/main.c ../SocketType.c lib.c -o bin/client && bin/client -p 5001 localhost
```
