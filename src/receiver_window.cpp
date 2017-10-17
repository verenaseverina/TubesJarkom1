#include "receiver_window.h"

void makeWindow(recvWindow &window, uint32_t maxSize){
	window.max_size = maxSize;
	window.current_size = 0;
	window.LAF = 0;
	window.LFR = 0; 
}

void growWindow(recvWindow &window){
	if(window.current_size < window.max_size){
		window.current_size++;
		window.LAF++;	
	}
}

void shrinkWindow(recvWindow &window){
	if(window.current_size > 0){
		window.current_size--;
		window.LFR++;	
	}
}

void incrementWindow(recvWindow &window){
	if(window.LAF < MAX_BUFFER_NUMBER){
		growWindow(window);
	}
	
	if((window.LFR < MAX_BUFFER_NUMBER)&&(window.LAF-window.LFR >= window.max_size)){
		shrinkWindow(window);
	}
}

void putPacketToBuffer(int sockfd, recvWindow &window, struct sockaddr_in client_addr){
	char dataBuffer;
	Packet * packet;
	bool confirmed[window.current_size];
	for(int i=0;i<window.current_size;i++){
		confirmed[i] = false;
	}
	while(true){
		recvfrom(sockfd, packet, 9, 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
		//itung checksum		

		dataBuffer = packet[6];
			
	}
}

void sendACK(uint32_t bufferNumber, int sockfd, uint8_t advWinSize, struct sockaddr_in client_addr){
	Ack ack;
	makeAck(ack, bufferNumber+1, advWinSize);
	Ack * _ack = &ack;
	sendto(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
}