
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "include/srvapi.h"
#include "include/server.h"
#include "include/comm.h"

static void spawn_worker(Packet *);

void
process_request(Packet * pckt_req, Packet * pckt_ans)
{
	int i;
	bool success;
	TableStatus status, nstatus[MAX_TABLES];


	switch (pckt_req->opcode) {
		case CHECK_TABLE:
			pckt_ans->data.ans_check_table.status =
					check_table(pckt_req->data.req_check_table.id);
			break;

		case CHECK_TABLES:
			check_tables(nstatus);
			for (i = 0; i < MAX_TABLES; i++) {
				pckt_ans->data.ans_check_tables.status[i] = nstatus[i];
			}
			break;

		case OCCUPY_TABLE:
			success = occupy_table(pckt_req->data.req_occupy_table.id);
			pckt_ans->data.ans_occupy_table.success = success;
			break;

		case FREE_TABLE:
			success = free_table(pckt_req->data.req_free_table.id);
			pckt_ans->data.ans_free_table.success = success;
			break;

		case RESERVE_TABLE:
			success = reserve_table(pckt_req->data.req_reserve_table.id);
			pckt_ans->data.ans_reserve_table.success = success;
			break;

		default:
			// TODO unknown operation code
			break;
	}
}

static void
spawn_worker(Packet * pckt_req)
{	
	Packet pckt_ans;

	int pid = fork();
	if (pid == 0) {
		printf("Processing command [%d]...\n", pckt_req->opcode);
		process_request(pckt_req, &pckt_ans);

		sleep(10);
		pk_send(pckt_req->pid, &pckt_ans, sizeof(pckt_ans));
		printf("Answer sent.\n");
	} else if (pid > 0) {
		// Do nothing, wait not needed because
		// sigaction set for SIGCHILD
	} else {
		// TODO handle error
	}
}

int
main(void)
{
	ssize_t n;
	Packet pckt_req;

	struct sigaction sigchld_action = {
		.sa_handler = SIG_DFL,
		.sa_flags = SA_NOCLDWAIT
	};
	// Avoid having to wait for child processes to exit
	sigaction(SIGCHLD, &sigchld_action, NULL);

	printf("Starting server...\n");
	if (init_server() == -1) {
		printf("Error initializing server.\n");
		return 1;
	}
	printf("Server started succesfully.\n");

	while (1) {
		// Receive requests and spawn workers
		n = pk_receive(SRV_ID, &pckt_req, sizeof(pckt_req));
		printf("%d byte/s received...\n", n);
	
		spawn_worker(&pckt_req);
	}

	return 0;
}

