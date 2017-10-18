#include "sender_window.h"

void senderMakeWindow(senderWindow &window, uint32_t maxSize)
{
	window.max_size = maxSize;
	window.current_size = 0;
	window.LFS = 0;
	window.LAR = 0;
	window.recv_adv_windsize = 256;
}

void senderReceiveACK(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr)
{
	Ack* _ack;
	Ack ack;

	recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	ack = *_ack;

	if(verifyAck(ack)) // check ack
	{ 
		window.recv_adv_windsize = ack.adv_windsize; // get adv_windsize from ack	
		window.LAR = ack.next_seqnum; // last ack received = ack next seqnum
		window.current_size = window.LFS - window.LAR; // update window size
	}
}

void senderSendPacket(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr, uint32_t seqnum, char data)
{
	Packet packet;
	Packet *_packet;

	makePacket(packet, seqnum, data);
	*packet = &packet;
	
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	window.LFS++; // update last frame received
	window.current_size++; // increment current size
}