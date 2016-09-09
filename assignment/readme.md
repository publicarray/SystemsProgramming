cc server/main.c ../SocketType.c lib.c ../01StringAndList/String.c server/ls.c server/cat.c -o bin/server && bin/server -p5001
cc client/main.c ../SocketType.c lib.c -o bin/client && bin/client -p 5001 localhost

#win32 pipe
#createprocess 
