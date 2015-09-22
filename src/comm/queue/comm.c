
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#include "include/comm.h"

static key_t key_srv = 0x1337; // Queue from which server reads
static key_t key_cli = 0x1338; // Queue from which clients read

static int qin;
static int qout;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	int n;

	struct {
		long mtype;
		char mtext[200];
	} msg;

	// Set msg.mtype >= 1 to avoid msgsnd EINVAL error
	msg.mtype = (id == SRV_ID ? 1 : id);
	memcpy((char *) msg.mtext, pckt, nbytes);

	n = msgsnd(qout, &msg, sizeof(Packet), 0);

	return (n == -1 ? -1 : nbytes);
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int n;
	// Server will read like a normal queue with t = 0
	// Client will read its packets only with t = id
	long t = (id == SRV_ID ? 0 : id);

	struct {
		long mtype;
		char mtext[200];
	} msg;

	n = msgrcv(qin, &msg, nbytes, t, 0);
	if (n != -1)
		memcpy(pckt, (char *) msg.mtext, nbytes);

	return n;
}

int
init_server(void)
{
	if ((qin = msgget(key_srv, 0666 | IPC_CREAT)) == -1) {
		printf("Error while getting queue.\n");
		return -1;
	}

	if ((qout = msgget(key_cli, 0666 | IPC_CREAT)) == -1) {
		printf("Error while getting queue.\n");
		return -1;
	}

	return 0;
}

int
init_client(void)
{
	if ((qin = msgget(key_cli, 0666 | IPC_CREAT)) == -1) {
		printf("Error while getting queue.\n");
		return -1;
	}

	if ((qout = msgget(key_srv, 0666 | IPC_CREAT)) == -1) {
		printf("Error while getting queue.\n");
		return -1;
	}
	return 0;
}
