#include <stdio.h>

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
	Ack ack;
	socklen_t server_len = sizeof(server_addr);

	printf("Waiting for ACK from receiver...\n");
	int _recv = recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, &server_len);
	printf("ACK received. ACK details : next_seqnum = %d, adv_windsize = %d\n", ack.next_seqnum, ack.adv_windsize);
	printf("Verifying ACK...\n");

	if(_recv < 0) return -1;
	else
	{
		if(verifyAck(ack)) // check ack
		{
			window.recv_adv_windsize = ack.adv_windsize; // get adv_windsize from ack	
			window.LAR = ack.next_seqnum - 1; // last ack received = ack next seqnum
			
			if(window.LAR + window.max_size >= window.lastSeqNum) window.LFS = window.lastSeqNum; // update LFS
			else window.LFS = window.LAR + window.max_size;

			printf("ACK verified. Current window state : LFS = %d, LAR = %d, recv_adv_windsize = %d, max_size = %d\n\n", window.LFS, window.LAR, window.recv_adv_windsize, window.max_size);
		}
		else printf("ACK verification failed. Drop ACK.\n\n");

		if(window.LFS == window.LAR) return -1;
		else return 0;
	}
}

void senderSendPacket(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr, uint32_t seqnum, char data)
{
	Packet packet;

	makePacket(packet, seqnum, data);
	printf("Sending packet to receiver. Packet details : seqnum = %d, data = '%c'\n", seqnum, data);
	sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	printf("Packet sent. Current window state : LFS = %d, LAR = %d, recv_adv_windsize = %d, max_size = %d\n\n", window.LFS, window.LAR, window.recv_adv_windsize, window.max_size);
}