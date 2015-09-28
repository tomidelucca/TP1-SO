
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
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
	int n, len;
	struct sockaddr * addr;

	UdpPacket udp_pckt;
	memcpy(&(udp_pckt.pckt), pckt, sizeof(Packet));
	udp_pckt.checksum = djb2_hash((char *) pckt, sizeof(Packet));	

	if (id == SRV_ID) { // sending from client to server
		addr = (struct sockaddr *) &srvaddr;
		len = sizeof srvaddr;
	} else { // sending from server to client
		addr = (struct sockaddr *) &cliaddr;
		len = sizeof cliaddr;
	}

	n = sendto(sockfd, &udp_pckt, sizeof udp_pckt, 0, addr, len);
	if (n == -1) {
		printf("Error sending packet.\n");
		return -1;
	}

	return n;
}

int
pk_receive(int id, Packet * pckt, int nbytes)
{
	int n, len;
	struct sockaddr * addr;

	UdpPacket udp_pckt;
	
	if (id != SRV_ID) { // client receiving
		len = sizeof srvaddr;
		addr = (struct sockaddr *) &srvaddr;
	} else { // server receiving
		len = sizeof cliaddr;
		addr = (struct sockaddr *) &cliaddr;
	}

	n = recvfrom(sockfd, &udp_pckt, sizeof udp_pckt, 0, addr, (socklen_t *) &len);
	if (n == -1) {
		printf("Error receiving packet.\n");
		return -1;
	}

	unsigned long checksum;
	checksum = djb2_hash((char *) &(udp_pckt.pckt), sizeof(Packet));
	if (checksum == udp_pckt.checksum) {
		memcpy(pckt, &(udp_pckt.pckt), sizeof(Packet));
	} else {
		return -1;
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

	struct timeval tv = {
		.tv_sec = 30,
		.tv_usec = 0
	};
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(tv));

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
