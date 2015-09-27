#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "include/comm.h"

#define ever (;;)
#define BUFFER_SIZE 512

static int delete_first_n_bytes(FILE *f, int n, const char* f_path);

static const char *path_fmt = "/tmp/file_%d.bin";
static const char *srv_path = "/tmp/file_srv.bin";
static const char *srv_pid_path = "/tmp/file_srv_pid";

static int srv_pid;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	FILE *fp;

	if(id == SRV_ID){ 	// sending from cli
		fp = fopen(srv_path, "ab");
	} else {			// sending from srv
		char path[50] = {0};
		sprintf(path, path_fmt, id);
		fp = fopen(path, "wb");
	}

	fwrite(pckt, nbytes, 1, fp);

	if(id == SRV_ID){
		kill(srv_pid, SIGUSR1);
	} else {
		kill(id, SIGUSR2);
	}

	fclose(fp);

	return nbytes;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int sig;
	FILE *fp;

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);						// allow program to be closed
	sigaddset(&set, (id==SRV_ID)?SIGUSR1:SIGUSR2);

	for ever{
		sigwait(&set, &sig);
		if(!(sig == SIGUSR1 || sig == SIGUSR2)){	// forward other signal
			kill(getpid(), sig);	
		}
		break;
	}

	if(id == SRV_ID){
		fp = fopen(srv_path, "rb");
	} else {
		char path[50] = {0};
		sprintf(path, path_fmt, getpid());
		fp = fopen(path, "rb");
	}

	fread(pckt, nbytes, 1, fp);

	if(id == SRV_ID){
		delete_first_n_bytes(fp, nbytes, srv_path);
	} 

	fclose(fp);

	return nbytes;
}

int
init_client(void)
{
	// get server pid
	FILE *fp = fopen(srv_pid_path, "r");

	if (fp!=0){
		fscanf(fp, "%d", &srv_pid);
	} else {
		return -1;
	}

	return 0;
}

int
init_server(void)
{
	// delete previously created srv file if exists
	fclose(fopen(srv_path, "w"));

	// store server pid to be referenced by clients
	FILE *fp = fopen(srv_pid_path, "w");
	
	if (fp!=0){
		fprintf(fp, "%d", getpid());
		fclose(fp);
	} else {
		return -1;
	}

	return 0;
}

/** 
 *	Deletes the first n bytes from f
 *
 *	@param	f File
 *	@param	n Number of bytes to delete
 *	@param	f_path Path of the f
 *
 */

static int
delete_first_n_bytes(FILE *f, int n, const char* f_path){
	
	char buffer[BUFFER_SIZE];

	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	fseek(f, n, SEEK_SET);

	fread(buffer, length, 1, f);

	freopen(f_path, "wb", f);

	fwrite(buffer, length-n, 1, f);

	return n;
}