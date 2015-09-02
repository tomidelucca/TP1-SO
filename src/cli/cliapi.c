
#include <cliapi.h>
#include <comm.h>

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

	pckt_req.pid = pid;
	pckt_req.opcode = opcode;
}

/**
 *	Send the specified packet to the server
 *
 *	@param	pckt Pointer to the packet to serialize and send to the server
 */
static void
send_client_packet(Packet * pckt)
{
	pk_send(SRV_ID, pckt, sizeof Packet);
}

TableStatus
check_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, CHECK_TABLE);
	pckt_req.data.req_check_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive(pid, &pckt_ans, sizeof Packet);

	return pckt_ans.data.ans_check_table.status;
}

TableStatus *
check_tables()
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, CHECK_TABLES);

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof Packet);

	return pckt_ans.data.ans_check_tables.status;
}

bool
occupy_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, OCCUPY_TABLE);
	pckt_req.data.req_occupy_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof Packet);

	return pckt_ans.data.ans_occupy_table.success;
}

bool
free_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pckt_req, FREE_TABLE);
	pckt_req.data.req_free_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof Packet);

	return pckt_ans.data.ans_free_table.success;
}

bool
reserve_table(int id)
{
	Packet pckt_req, pckt_ans;

	create_packet(&pctk_req, RESERVE_TABLE);
	pckt_req.data.req_reserve_table.id = id;

	send_client_packet(&pckt_req);
	pk_receive((int) getpid(), &pckt_ans, sizeof Packet);

	return pckt_ans.data.ans_reserve_table.success;
}
