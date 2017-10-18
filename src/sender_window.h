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
	uint32_t LFS; // last frame sent
	uint32_t LAR; // last ack received
	uint32_t max_size; // sender window size
	uint32_t current_size;
	uint8_t recv_adv_windsize; // receiver's adv window size
} senderWindow;

void senderMakeWindow(senderWindow &window, uint32_t maxSize);
void senderReceiveACK(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr);
void senderSendPacket(senderWindow &window int &sockfd, struct sockaddr_in &server_addr, uint32_t seqnum, char data);