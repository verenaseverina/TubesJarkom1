#pragma once

#include <stdint.h>
#include <strings.h>

typedef struct {
    char ACK; // ACK
    uint32_t next_seqnum; // next sequence number
    uint8_t adv_windsize; // advertised window size
    uint8_t checksum; // checksum
} __attribute__((packed)) Ack;

uint8_t computeAckChecksum(uint32_t nextSeqNum, uint8_t advWindowSize);

void makeAck(Ack &ack, uint32_t nextSeqNum, uint8_t advWindowSize);
void makeFileSizeAck(Ack &ack, uint32_t size);
//void makeStartFileAck(Ack &ack);
//void makeEndFileAck(Ack &ack);

bool verifyAck(Ack &ack);
bool verifyFileSizeAck(Ack &ack, uint32_t size);
//bool verifyStartFileAck(Ack &ack);
//bool verifyEndFileAck(Ack &ack);