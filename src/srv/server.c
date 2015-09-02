
#include <server.h>
#include <srvapi.h>
#include <comm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(void)
{
	Packet pckt_req, pckt_ans;

	while (1) {
		// Receive requests and create workers
		pk_receive(SRV_ID, &pckt_req, sizeof Packet);
		// TODO spawn worker to process the received packet
		process_command(&pckt_req, &pckt_ans);
		pk_send(pckt_req.pid, &pckt_ans, sizeof Packet);
	}

	return 0;
}
