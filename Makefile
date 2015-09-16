
CC=gcc
CFLAGS=-Wall -lrt -lpthread
INCLUDE=-I .

all: client server

client:
	gcc $(INCLUDE) $(CFLAGS) src/cli/client.c src/cli/cliapi.c src/comm/$(IPC)/comm.c -o cli
	
server:
	gcc $(INCLUDE) $(CFLAGS) src/srv/server.c src/srv/srvapi.c src/comm/$(IPC)/comm.c -o srv

fcntl:
	gcc $(INCLUDE) src/cli/client.c src/srv/server.c -o cli
