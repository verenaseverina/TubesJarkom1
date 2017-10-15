#pragma once

#include <stdint.h>

typedef struct {
    const char ACK = 0x6; // ACK
    uint32_t next_seqnum; // next sequence number
    uint8_t adv_windsize; // advertised window size
    uint8_t checksum; // checksum
} __attribute__((packed)) Ack;

Ack makeAck(uint32_t nextSeqNum, uint8_t advWindowSize);