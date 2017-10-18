#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ack.h"
#include "packet.h"

typedef struct {
	uint32_t LAF; // last acceptable frame
	uint32_t LFR; // last frame received
	uint32_t max_size; // receiver window size
	uint32_t current_size;
	char receiverBuffer[256]; // receiver buffer for temporary storage of unordered packets
	int lastIndex; // largest non-empty index in receiver buffer, for moving data from recv buffer to file buffer
	int bufferCount; // count the number of non-empty buffer in recv buffer
} recvWindow;

void receiverMakeWindow(recvWindow &window, uint32_t maxSize);
void receiverReceivePacket(recvWindow &window, int &sockfd, struct sockaddr_in &client_addr, socklen_t &client_len, char* &file_buffer);
void receiverSendACK(recvWindow &window, int &sockfd, struct sockaddr_in &client_addr, uint32_t nextSeqNum);