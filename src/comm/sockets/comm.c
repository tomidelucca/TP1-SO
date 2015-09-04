
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>

#include "include/comm.h"

#define SRV_IP		"127.0.0.1"
#define SRV_PORT	1337

static void init_server(void);
static void init_client(void);

static bool established_connection = false;
static int sockfd;
static struct sockaddr_in srvaddr, cliaddr;

void
pk_send(int id, Packet * pckt, int nbytes)
{
	if (!established_connection)
		(id == SRV_ID) ? init_client() : init_server();

	if (id == SRV_ID) // sending from client to server
		sendto(sockfd, pckt, nbytes, 0, (struct sockaddr *) &srvaddr, sizeof srvaddr);
	else // sending from server to client
		sendto(sockfd, pckt, nbytes, 0, (struct sockaddr *) &cliaddr, sizeof cliaddr);
}

void
pk_receive(int id, Packet * pckt, int nbytes)
{
	int n, len;

	if (!established_connection)
		(id == SRV_ID) ? init_server() : init_client();

	if (id != SRV_ID) { // receiving from the server
		len = sizeof srvaddr;
		n = recvfrom(sockfd, pckt, nbytes, 0, (struct sockaddr *) &srvaddr, &len);
	} else { // receiving from a client
		len = sizeof cliaddr;
		n = recvfrom(sockfd, pckt, nbytes, 0, (struct sockaddr *) &cliaddr, &len);
	}
}

static void
init_server(void)
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // open udp socket
	if (sockfd == -1)
		printf("Error opening udp server socket. (%d)\n", errno);

	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(SRV_PORT);

	if (bind(sockfd, (struct sockaddr *) &srvaddr, sizeof srvaddr) != -1)
		established_connection = true; // flag the connection
	else
		printf("Error binding socket. (%d)\n", errno);
}


static void
init_client(void)
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // open udp socket
	if (sockfd == -1)
		printf("Error opening udp client socket. (%d)\n", errno);

	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SRV_PORT);
	srvaddr.sin_addr.s_addr = inet_addr(SRV_IP);

	established_connection = true; // flag the connection
}
