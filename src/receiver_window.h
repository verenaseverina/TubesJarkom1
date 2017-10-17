#pragma once

#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ack.h"
#include "packet.h"
#define MAX_BUFFER_NUMBER 256 //sequence/frame number start from 0, 1 frame 9 bytes

typedef struct {
	uint32_t LAF;
	uint32_t LFR;
	uint32_t max_size;
	uint32_t current_size;
} recvWindow;

char receiverBuffer[256];

void makeWindow(recvWindow &window, uint32_t maxSize);
void growWindow(recvWindow &window);
void shrinkWindow(recvWindow &window);
void incrementWindow(recvWindow &window);
void putPacketToBuffer(int sockfd, recvWindow &window, struct sockaddr_in client_addr);
void sendACK(uint32_t frameNumber, int sockfd, struct sockaddr_in client_addr);

