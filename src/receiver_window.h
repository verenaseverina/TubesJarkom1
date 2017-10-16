#pragma once

#define MAX_FRAME_NUMBER 27 //sequence/frame number start from 0

typedef struct {
	int LAF;
	int LFR;
	int max_size;
	int current_size;
} recvWindow;

recvWindow makeWindow(int maxSize);
void growWindow(recvWindow &window);
void shrinkWindow(recvWindow &window);
void incrementWindow(recvWindow &window);