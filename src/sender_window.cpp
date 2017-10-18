#include "sender_window.h"

void senderMakeWindow(senderWindow &window, uint32_t maxSize, uint32_t lastSeqNum)
{
	window.max_size = maxSize;
	window.LFS = maxSize;
	window.LAR = 0;
	window.recv_adv_windsize = 256;
	window.lastSeqNum = lastSeqNum;
}

int senderReceiveACK(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr)
{
	Ack* _ack;
	Ack ack;
	socklen_t server_len = sizeof(server_addr);

	int _recv = recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, &server_len);

	if(_recv < 0) return -1;
	else
	{
		ack = *_ack;

		if(verifyAck(ack)) // check ack
		{ 
			window.recv_adv_windsize = ack.adv_windsize; // get adv_windsize from ack	
			window.LAR = ack.next_seqnum - 1; // last ack received = ack next seqnum
			
			if(window.LAR + window.max_size >= window.lastSeqNum) window.LFS = window.lastSeqNum; // update LFS
			else window.LFS = window.LAR + window.max_size;
		}

		return 0;
	}
}

void senderSendPacket(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr, uint32_t seqnum, char data)
{
	Packet packet;
	Packet *_packet;

	makePacket(packet, seqnum, data);
	_packet = &packet;
	
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
}