
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "include/comm.h"

static const char *path_fmt = "/tmp/fifo_%d";
static const char *srv_path = "/tmp/fifo_srv";

// Read fifo
static int fd = -1;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	int wr_fd;
	char path[50] = {0};

	// Set the path of the fifo
	if (id == SRV_ID) {
		strcpy(path, srv_path);
	} else {
		sprintf(path, path_fmt, id);
	}

	wr_fd = open(path, O_WRONLY);
	if (wr_fd == -1) {
		// TODO handle error
	}
	
	return write(wr_fd, pckt, nbytes);
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int n;

	if (fd == -1) {
		// TODO handle error
	}

	// Read till there is something to read
	while ((n = read(fd, pckt, nbytes)) != nbytes);

	return n;
}

void
init_server(void)
{
	// Check if fifo is created, create it if not
	if (access(srv_path, F_OK) == -1 && mkfifo(srv_path, S_IRWXU) == -1) {
		// TODO handle error
		printf("Error with mkfifo\n");
	}

	fd = open(srv_path, O_RDONLY);
	if (fd == -1) {
		printf("Error opening server fifo.\n");
	}
}

void
init_client(void)
{
	char path[50] = {0};

	sprintf(path, path_fmt, (int) getpid());
	if (access(path, F_OK) == -1 && mkfifo(path, S_IRWXU) == -1) {
		// TODO handle error
	}
	printf("fifo created at %s\n", path);

	fd = open(path, O_RDONLY | O_NONBLOCK);
	if (fd == -1) {
		printf("Error opening client fifo.\n");
	}
}

