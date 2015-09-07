
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/server.h"
#include "include/comm.h"

/**
 *	Create a generic packet
 *
 *	@param	pckt Pointer to the packet to set up
 *	@param	opcode Operation code of the packet
 */
static void
create_packet(Packet * pckt, int opcode)
{
	int pid = (int) getpid();

	pckt->pid = pid;
	pckt->opcode = opcode;
}

/**
 *	Send the specified packet to the server
 *
 *	@param	pckt Pointer to the packet to serialize and send to the server
 */
static int
send_client_packet(Packet * pckt)
{
	return pk_send(SRV_ID, pckt, sizeof *pckt);
}

int
check_table(int id, TableStatus *status)
{
	Packet pckt_req, pckt_ans;
	
	create_packet(&pckt_req, CHECK_TABLE);
	pckt_req.data.req_check_table.id = id;

	if (send_client_packet(&pckt_req) == -1)
		return -1;

	if (pk_receive((int) getpid(), &pckt_ans, sizeof(Packet)) == -1)
		return -1;

	*status = pckt_ans.data.ans_check_table.status;

	return 0;
}

int
check_tables(TableStatus *status)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, CHECK_TABLES);

	if (send_client_packet(&pckt_req) == -1)
		return -1;

	if (pk_receive((int) getpid(), &pckt_ans, sizeof(Packet)) == -1)
		return -1;

	memcpy(status, pckt_ans.data.ans_check_tables.status,
			sizeof(TableStatus) * MAX_TABLES);
	return 0;
}

int
occupy_table(int id, bool *success)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, OCCUPY_TABLE);
	pckt_req.data.req_occupy_table.id = id;

	if (send_client_packet(&pckt_req) == -1)
		return -1;
	
	if (pk_receive((int) getpid(), &pckt_ans, sizeof(Packet)) == -1)
		return -1;

	*success = pckt_ans.data.ans_occupy_table.success;

	return 0;
}

int
free_table(int id, bool *success)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, FREE_TABLE);
	pckt_req.data.req_free_table.id = id;

	if (send_client_packet(&pckt_req) == -1)
		return -1;

	if (pk_receive((int) getpid(), &pckt_ans, sizeof(Packet)) == -1)
		return -1;

	*success = pckt_ans.data.ans_free_table.success;

	return 0;
}

int
reserve_table(int id, bool *success)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, RESERVE_TABLE);
	pckt_req.data.req_reserve_table.id = id;

	if (send_client_packet(&pckt_req) == -1)
		return -1;

	if (pk_receive((int) getpid(), &pckt_ans, sizeof(Packet)) == -1)
		return -1;

	*success = pckt_ans.data.ans_reserve_table.success;

	return 0;
}

