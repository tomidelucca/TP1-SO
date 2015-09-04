
#include "include/srvapi.h"
#include "include/server.h"
#include "include/comm.h"

int
main(void)
{
	Packet pckt_req, pckt_ans;

	while (1) {
		// Receive requests and create workers
		pk_receive(SRV_ID, &pckt_req, sizeof Packet);
		// TODO spawn worker to process the received packet
		int pid = fork();

		if (pid == 0) {
			// child
			process_command(&pckt_req, &pckt_ans);
			pk_send(pckt_req.pid, &pckt_ans, sizeof Packet);
			exit(0);
		}
	}

	return 0;
}

void
process_request(Packet * pckt_req, Packet * pckt_ans)
{
	switch (pckt_req->opcode) {
		case CHECK_TABLE:
			TableStatus status = check_table(
				pckt_req->data.req_check_table.id
			);

			pckt_ans->data.ans_check_table.status = status;
			break;

		case CHECK_TABLES:
			TableStatus[MAX_TABLES] status = check_tables();

			pckt_ans->data.ans_check_tables.status = status;
			break;

		case OCCUPY_TABLE:
			bool success = occupy_table(
				pckt_req->data.req_occupy_table.id
			);

			pckt_ans->data.ans_occupy_table.success = success;
			break;

		case FREE_TABLE:
			bool success = free_table(
				pckt_req->data.req_free_table.id
			);

			pckt_ans->data.ans_free_table.success = success;
			break;

		case RESERVE_TABLE:
			bool success = reserve_table(
				pckt_req->data.req_reserve_table.id
			);

			pckt_ans->data.ans_reserve_table.success = success;
			break;

		default:
			// TODO unknown operation code
			break;
	}
}
