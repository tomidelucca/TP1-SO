
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#include "include/comm.h"

#define SRV_IP		"127.0.0.1"
#define SRV_PORT	1337

typedef struct {
	Packet pckt;
	unsigned long checksum;
} UdpPacket;

static unsigned long djb2_hash(const char *, int);

static int sockfd;
static struct sockaddr_in srvaddr, cliaddr;

int
pk_send(int id, Packet * pckt, int nbytes)
{
	int n;

	UdpPacket udp_pckt;
	memcpy(&(udp_pckt.pckt), pckt, sizeof(Packet));
	udp_pckt.checksum = djb2_hash((char *) pckt, sizeof(Packet));	

	if (id == SRV_ID) {// sending from client to server
		n = sendto(sockfd, &udp_pckt, sizeof udp_pckt, 0, (struct sockaddr *) &srvaddr, sizeof srvaddr);
	} else {// sending from server to client
		n = sendto(sockfd, &udp_pckt, sizeof udp_pckt, 0, (struct sockaddr *) &cliaddr, sizeof cliaddr);
	}

	return n;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int n, len;

	UdpPacket udp_pckt;
	
	if (id != SRV_ID) { // receiving from the server
		len = sizeof srvaddr;
		n = recvfrom(sockfd, &udp_pckt, sizeof udp_pckt, 0, (struct sockaddr *) &srvaddr, &len);
	} else { // receiving from a client
		len = sizeof cliaddr;
		n = recvfrom(sockfd, &udp_pckt, sizeof udp_pckt, 0, (struct sockaddr *) &cliaddr, &len);
	}

	unsigned long checksum;
	checksum = djb2_hash((char *) &(udp_pckt.pckt), sizeof(Packet));
	if (checksum == udp_pckt.checksum) {
		memcpy(pckt, &(udp_pckt.pckt), sizeof(Packet));
	} else {
		// TODO what should we do when the packet is corrupted?
	}

	return n;
}

int
init_server(void)
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // open udp socket
	if (sockfd == -1) {
		printf("Error opening udp server socket. (%d)\n", errno);
		return -1;
	}

	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(SRV_PORT);

	if (bind(sockfd, (struct sockaddr *) &srvaddr, sizeof srvaddr) == -1) {
		printf("Error binding socket. (%d)\n", errno);
		return -1;
	}

	return 0;
}


int
init_client(void)
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // open udp socket
	if (sockfd == -1) {
		printf("Error opening udp client socket. (%d)\n", errno);
		return -1;
	}

	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SRV_PORT);
	srvaddr.sin_addr.s_addr = inet_addr(SRV_IP);

	return 0;
}

/**
 *	Create a hash given `amount` bytes, starting at `ptr`
 *
 *	@param	ptr Start of the stream of bytes
 *	@param	amount Amount of bytes
 *	@return	Hashed bytes
 */
static unsigned long
djb2_hash(const char * ptr, int amount)
{
	unsigned long hash = 5381;
	int c;

	while (amount > 0) {
		c = *ptr++;
		hash = ((hash << 5) + hash) + c;
		amount--;
	}
	
	return hash;
}
