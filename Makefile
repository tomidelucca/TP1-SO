
CC=gcc
CFLAGS=-c -Wall

all: client server

client:
	gcc -I . src/cli/client.c src/cli/cliapi.c src/comm/sockets/comm.c -o cli
	
server:
	gcc -I . src/srv/server.c src/srv/srvapi.c src/comm/sockets/comm.c -o srv

fifos: fifo_cli fifo_srv

fifo_cli:
	gcc -I . src/cli/client.c src/cli/cliapi.c src/comm/fifos/comm.c -o cli

fifo_srv:
	gcc -I . src/srv/server.c src/srv/srvapi.c src/comm/fifos/comm.c -o srv
