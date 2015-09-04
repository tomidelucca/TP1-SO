
#ifndef _SRVAPI_H_
#define _SRVAPI_H_

/**
 *	Process the request and store the result into pckt_ans
 *
 *	@param	pckt_req Packet of the request
 *	@param	pckt_ans Packet of the answer
 *
 */
void process_request(Packet * pckt_req, Packet * pckt_ans);

#endif
