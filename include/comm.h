
#ifndef _COMM_H_
#define _COMM_H_

#include <stdbool.h>

#define SRV_ID		0
#define MAX_TABLES 	10

typedef enum { CHECK_TABLE, CHECK_TABLES, OCCUPY_TABLE, FREE_TABLE,
	RESERVE_TABLE } Opcode;

typedef enum { NON_EXISTENT, AVAILABLE, RESERVED, OCCUPIED } TableStatus;

// Request arguments
typedef struct {
	int id;
} CheckTableReq;

typedef struct {
	int id;
} OccupyTableReq;

typedef struct {
	int id;
} FreeTableReq;

typedef struct {
	int id;
} ReserveTableReq;

// Answer arguments
typedef struct {
	int id;
	TableStatus status;
} CheckTableAns;

typedef struct {
	TableStatus status[MAX_TABLES];
} CheckTablesAns;

typedef struct {
	int id;
	bool success;
} OccupyTableAns;

typedef struct {
	int id;
	bool success;
} FreeTableAns;

typedef struct {
	int id;
	bool success;
} ReserveTableAns;

typedef union {
	OccupyTableReq	req_occupy_table;
	FreeTableReq	req_free_table;
	ReserveTableReq	req_reserve_table;
	CheckTableReq	req_check_table;
	OccupyTableAns	ans_occupy_table;
	FreeTableAns	ans_free_table;
	ReserveTableAns	ans_reserve_table;
	CheckTableAns	ans_check_table;
	CheckTablesAns	ans_check_tables;
} Data;

typedef struct {
	int pid;
	int opcode;
	Data data;
} Packet;

/**
 * 	Reads nbytes into pckt whenever there is a packet sent to id
 *
 *	@param	id Destinatary of the packet
 *	@param	pckt Packet structure to store serialized bytes
 *	@param	nbytes Amount of bytes to read
 *	@return	number of bytes received
 */
int pk_receive(int id, Packet * pckt, int nbytes);

/**
 *	Send nbytes of the pckt to process identified by id
 *
 *	@param	id Destinatary of the packet, it should be pid of the client or
 *				SRV_ID for the server
 *	@param	pckt Packet structure to store serialized bytes
 *	@param	nbytes Amount of bytes to read
 *	@return	number of bytes sent
 */
int pk_send(int id, Packet * pckt, int nbytes);

/**
 *	Initialize client connection
 *	@return -1 on failure
 */
int init_client();

/**
 *	Initialize server connection
 *	@return -1 on failure
 */
int init_server();

#endif
