
CC=gcc
CFLAGS=-c -Wall

all: client server

client:
	gcc -I . src/cli/client.c src/cli/cliapi.c src/comm/sockets/comm.c -o cli
	
server:
	gcc -I . src/srv/server.c src/srv/srvapi.c src/comm/sockets/comm.c -o srv
