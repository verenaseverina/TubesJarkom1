#include "receiver_window.h"

recvWindow makeWindow(int maxSize){
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
	if(window.LAF < MAX_FRAME_NUMBER){
		growWindow(window);
	}
	
	if((window.LFR < MAX_FRAME_NUMBER)&&(window.LAF >= window.max_size)){
		shrinkWindow(window);
	}
}