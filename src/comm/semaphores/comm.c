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

#define SRV_KEY	"srv_key"

typedef struct {
	Packet packet;
	bool empty;
} ShmPacket;

#define SHM_PCKT_SIZE	sizeof (ShmPacket)

ShmPacket *init_packet_memory(const char* key);
sem_t *init_mutex(const char* key);

static sem_t *first;
static ShmPacket *shm_request;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	/*if(id == SRV_ID){	// sending from client

	} else {			// sending form server

	}*/

	sem_wait(first);
	memcpy(&shm_request->packet, pckt, SHM_PCKT_SIZE);
	shm_request->empty = false;
	sem_post(first);

	return SHM_PCKT_SIZE;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{

	bool is_empty = false;

	do {
	
		sem_wait(first);

		/*if(id == SRV_ID){	// receiving in server

		} else {			// receiving in client

		}*/

		if((is_empty = shm_request->empty) == false) {
			memcpy(pckt, &shm_request->packet, SHM_PCKT_SIZE);
			shm_request->empty = true;
		}

		sem_post(first);

	} while (is_empty);

	return SHM_PCKT_SIZE;
}

int
init_client(void)
{
	shm_request = init_packet_memory(SRV_KEY);
	first = init_mutex(SRV_KEY);

	return 0;
}

int
init_server(void)
{

	sem_unlink(SRV_KEY);	// cleans old semaphore if was created (other way?)
	shm_request = init_packet_memory(SRV_KEY);
	first = init_mutex(SRV_KEY);

	shm_request->empty = true;	// sets the newly created memory zone as empty (no current data to read)

	return 0;
}

/**
 *	Initializes the shared memory for ´key´
 *
 * 	@param	key Name for the mapped memory
 *	@return a fetched or newly created shared memory address
 */
ShmPacket *
init_packet_memory(const char* key)
{
	int fd;
	ShmPacket* mem;

	if ((fd = shm_open(key, O_RDWR | O_CREAT, 0666)) == -1){	// allocate shared memory
		printf("Error allocating shared memory\n");
		exit(1);
	}

	ftruncate(fd, SHM_PCKT_SIZE);	// resizes the shm to match the packet size

	if ( !(mem = mmap(NULL, SHM_PCKT_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) ){	// map shared memory
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
init_mutex(const char* key)
{
	sem_t *mutex;
	
	if ( !(mutex = sem_open(key, O_CREAT | O_RDWR, 0666, 1)) ){	// create mutex
		printf("Error opening mutex\n");
		exit(1);
	}

	return mutex;
}