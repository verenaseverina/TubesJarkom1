#include "receiver_window.h"

recvWindow makeWindow(uint32_t maxSize){
	recvWindow window;
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
	
	if((window.LFR < MAX_BUFFER_NUMBER)&&(window.LAF >= window.max_size)){
		shrinkWindow(window);
	}
}

void putPacketToBuffer(int sockfd){
	
}

void sendACK(uint32_t bufferNumber, int sockfd, uint8_t advWinSize){
	Ack ack = makeAck(bufferNumber+1, advWinSize);
	sendto(sockfd, ack, 9, 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
}