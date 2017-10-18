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
	uint32_t LFS; // last frame sent, start from 1
	uint32_t max_size; // sender window size, start from 1
	uint8_t recv_adv_windsize; // receiver's adv window size
	uint32_t lastSeqNum; // largest seqnum of packet
} senderWindow; // LAR < window <= LFS

void senderMakeWindow(senderWindow &window, uint32_t maxSize, uint32_t lastSeqNum);
void senderReceiveACK(senderWindow &window, int &sockfd, struct sockaddr_in &server_addr);
void senderSendPacket(senderWindow &window int &sockfd, struct sockaddr_in &server_addr, uint32_t seqnum, char data);