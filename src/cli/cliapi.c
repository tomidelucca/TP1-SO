
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/cliapi.h"
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
static void
send_client_packet(Packet * pckt)
{
	pk_send(SRV_ID, pckt, sizeof *pckt);
}

TableStatus
check_table(int id)
{
	Packet pckt_req, pckt_ans;
	
	create_packet(&pckt_req, CHECK_TABLE);
	pckt_req.data.req_check_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof(Packet));

	return pckt_ans.data.ans_check_table.status;
}

void
check_tables(TableStatus * status)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, CHECK_TABLES);

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof(Packet));

	memcpy(status, pckt_ans.data.ans_check_tables.status,
			sizeof(TableStatus) * MAX_TABLES);
}

bool
occupy_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, OCCUPY_TABLE);
	pckt_req.data.req_occupy_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof(Packet));

	return pckt_ans.data.ans_occupy_table.success;
}

bool
free_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, FREE_TABLE);
	pckt_req.data.req_free_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof(Packet));

	return pckt_ans.data.ans_free_table.success;
}

bool
reserve_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, RESERVE_TABLE);
	pckt_req.data.req_reserve_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof(Packet));

	return pckt_ans.data.ans_reserve_table.success;
}
