
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "include/comm.h"

static void fifo_signal(int);

static const char *path_fmt = "/tmp/fifo_%d";
static const char *srv_path = "/tmp/fifo_srv";

int
pk_send(int id, Packet * pckt, int nbytes)
{
	int fd, n;
	char path[50] = {0};

	// Set the path of the fifo
	if (id == SRV_ID) {
		strcpy(path, srv_path);
	} else {
		sprintf(path, path_fmt, id);
	}

	fd = open(path, O_WRONLY | O_NONBLOCK);
	if (fd == -1) {
		printf("\x1B[31m[ERROR]\x1B[0m Couldn't open fifo to write.\n");
		exit(1);
	}

	n = write(fd, pckt, nbytes);
	close(fd);

	return n;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int fd, n;
	char path[50] = {0};

	// Set the path of the fifo
	if (id == SRV_ID) {
		strcpy(path, srv_path);
	} else {
		sprintf(path, path_fmt, id);
	}

	fd = open(path, O_RDONLY);
	if (fd == -1) {
		printf("\x1B[31m[ERROR]\x1B[0m Couldn't open fifo to read.\n");
		return -1;
	}

	// Read till there is something to read
	while ((n = read(fd, pckt, nbytes)) != nbytes);
	close(fd);

	return n;
}

int
init_server(void)
{
	// Check if fifo is created, create it if not
	if (access(srv_path, F_OK) == -1 && mkfifo(srv_path, S_IRWXU) == -1) {
		printf("\x1B[31m[ERROR]\x1B[0m Error with mkfifo.\n");
		return -1;
	}

	signal(SIGPIPE, fifo_signal);

	return 0;
}

int
init_client(void)
{
	char path[50] = {0};

	sprintf(path, path_fmt, (int) getpid());
	if (access(path, F_OK) == -1 && mkfifo(path, S_IRWXU) == -1) {
		printf("\x1B[31m[ERROR]\x1B[0m Error with mkfifo.\n");
		return -1;
	}

	return 0;
}


static void
fifo_signal(int sig)
{
	printf("Fifo is dead");
	exit(0);
}
