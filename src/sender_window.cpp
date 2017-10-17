#include "sender_window.h"

senderWindow senderMakeWindow(uint32_t maxSize){
	window.max_size = maxSize;
	window.current_size = 0;
	window.LFS = 0;
	window.LAR = 0; 
}

void senderGrowWindow(senderWindow &window, int delta) {
	if (window.current_size < window.max_size) {
		window.current_size += delta;
		window.LFS += delta;	
	}
}

void senderShrinkWindow(senderWindow &window, int delta) {
	if (window.current_size > 0) {
		window.current_size -= delta;
		window.LAR += delta;	
	}
}

void senderIncrementWindow(senderWindow &window, int delta) {
	if (window.LFS < MAX_BUFFER_NUMBER) {
		senderGrowWindow(window,delta);
	}
	
	if ((window.LAR < MAX_BUFFER_NUMBER) && (window.LFS - window.LAR >= window.max_size)) {
		senderShrinkWindow(window,delta);
	}
}

void senderReceiveACK(int sockfd, senderWindow &window, struct sockaddr_in server_addr) {
	Ack * _ack;
	Ack ack;
	uint8_t adv_windsize;

	while (true) {
		recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
		
		ack = *_ack;			
		if (verifyAck(ack)) { //cek ack
			adv_windsize = ack.adv_windsize; //ambil adv_windsize dari ack
			// kalo adv_windsize, tunggu sampe timeout terus baru ngirim lagi
			window.LAR = ack.next_seqnum;
		}	
	}
}

void senderSendPacket(senderWindow &window, uint32_t seqnum, int sockfd, struct sockaddr_in server_addr, char data) {
	Packet packet;
	makePacket(Packet &packet, seqnum, data);

	Packet * _packet = $packet;
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	window.LFS++;
}