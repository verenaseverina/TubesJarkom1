#include "receiver_window.h"

char receiverBuffer[256];

void makeWindow(recvWindow &window, uint32_t maxSize){
	window.max_size = maxSize;
	window.current_size = 0;
	window.LAF = 0;
	window.LFR = 0; 
}

void growWindow(recvWindow &window, int delta){
	if(window.current_size < window.max_size){
		window.current_size += delta;
		window.LAF += delta;	
	}
}

void shrinkWindow(recvWindow &window, int delta){
	if(window.current_size > 0){
		window.current_size -= delta;
		window.LFR += delta;	
	}
}

void incrementWindow(recvWindow &window, int delta){
	if(window.LAF < MAX_BUFFER_NUMBER){
		growWindow(window,delta);
	}
	
	if((window.LFR < MAX_BUFFER_NUMBER)&&(window.LAF-window.LFR >= window.max_size)){
		shrinkWindow(window,delta);
	}
}

void putPacketToBuffer(int sockfd, recvWindow &window, struct sockaddr_in client_addr, char *&fileBuffer){
	char dataBuffer;
	Packet * _packet;
	Packet packet;

	int lastIndex;//penunjuk ke index terbesar di receiver buffer
	uint8_t advWinSize = window.current_size;
	while(true){
		recvfrom(sockfd, _packet, 9, 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
		packet = *_packet;
						
		if(verifyPacket(packet)){ //cek paket
			dataBuffer = packet.data; //ambil data dari paket
			if(lastIndex == 0){//cek kalau ada yang data yang belum diterima
				fileBuffer[packet.seqnum-1] = dataBuffer;
				//kirim ack
				sendACK(packet.seqnum-1,sockfd,advWinSize--,(struct sockaddr* ) &client_addr);
			}else{
				lastIndex = packet.seqnum - window.LFR - 1;				
				receiverBuffer[lastIndex] = dataBuffer;//masukin ke receiver buffer
				//kirim ack nomor paket yang ilang(?)
				
			}
			
		}
			
	}
}

void sendACK(uint32_t bufferNumber, int sockfd, uint8_t advWinSize, struct sockaddr_in client_addr){
	Ack ack;
	// belum dihandle buat yang ack kalau masih ada paket yang ilang : kirim nomor diri sendiri
	makeAck(ack, bufferNumber+1, advWinSize);
	Ack * _ack = &ack;
	sendto(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
}