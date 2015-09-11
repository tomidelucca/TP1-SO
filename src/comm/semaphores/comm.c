#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/comm.h"

#define SHM_RQ_KEY	"SHM_RQ"

#define SEM1_KEY	"SEM1"
#define SEM2_KEY	"SEM2"

#define PCKT_SIZE	sizeof (Packet)

Packet *init_packet_memory(const char* key);
sem_t *init_semaphore(const char* key, int initial_value);
void up(sem_t *sem);
void down(sem_t *sem);

static sem_t *s1;
static sem_t *s2;

static sem_t *r;

static const char *response_fmt = "shm_%d";

static Packet *shm_request;
static Packet *shm_response;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	char key[50] = {0};
	sprintf(key, response_fmt, ((id == SRV_ID)?getpid():id));
	shm_response = init_packet_memory(key);
	r = init_semaphore(key, 1);

	if(id == SRV_ID){	// sending from client
		down(s1);
		memcpy(shm_request, pckt, PCKT_SIZE);
		down(r);
		up(s2);
	} else {			// sending form server
		memcpy(shm_response, pckt, PCKT_SIZE);
		up(r);
	}

	return PCKT_SIZE;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	if(id == SRV_ID){	// receiving in server
		down(s2);
		memcpy(pckt, shm_request, PCKT_SIZE);
		up(s1);
	} else {			// receiving in client
		down(r);
		memcpy(pckt, shm_response, PCKT_SIZE);
		up(r);
		char key[50] = {0};
		sprintf(key, response_fmt, id);
		sem_unlink(key);	// removes the response semaphore
		shm_unlink(key);	// removes the response shared memory
	}

	return PCKT_SIZE;
}

int
init_client(void)
{
	shm_request = init_packet_memory(SHM_RQ_KEY);
	s1 = init_semaphore(SEM1_KEY, 1);
	s2 = init_semaphore(SEM2_KEY, 0);

	return 0;
}

int
init_server(void)
{
	sem_unlink(SEM1_KEY);	
	sem_unlink(SEM2_KEY);	// cleans old semaphore if was created

	shm_request = init_packet_memory(SHM_RQ_KEY);
	s1 = init_semaphore(SEM1_KEY, 1);
	s2 = init_semaphore(SEM2_KEY, 0);

	return 0;
}

/**
 *	Initializes the shared memory for ´key´
 *
 * 	@param	key Name for the mapped memory
 *	@return a fetched or newly created shared memory address
 */
Packet *
init_packet_memory(const char* key)
{
	int fd;
	Packet* mem;

	if ((fd = shm_open(key, O_RDWR | O_CREAT, 0666)) == -1){	// allocate shared memory
		printf("Error allocating shared memory\n");
		exit(1);
	}

	ftruncate(fd, PCKT_SIZE);	// resizes the shm to match the packet size

	if ( !(mem = mmap(NULL, PCKT_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) ){	// map shared memory
		printf("Error attaching the segment\n");
		exit(1);
	}

	close(fd);

	return mem;
}

/**
 *	Initializes the semaphore for ´key´
 *
 * 	@param	key Name for the semaphore
 *	@return a fetched or newly created semaphore
 */
sem_t *
init_semaphore(const char* key, int initial_value)
{
	sem_t *sem;
	
	if ( !(sem = sem_open(key, O_CREAT | O_RDWR, 0666, initial_value)) ){	// create semaphore
		printf("Error opening mutex\n");
		exit(1);
	}

	return sem;
}

/**
 *	Increments by one the value of the semaphore
 *
 *	@param sem The semaphore to increment
 */
void
up(sem_t *sem)
{
	sem_post(sem);
}

/**
 *	Decrements by one the value of the semaphore
 *
 *	@param sem The semaphore to decrement
 */
void
down(sem_t *sem)
{
	sem_wait(sem);
}