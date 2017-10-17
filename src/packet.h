#pragma once

#include <stdint.h>

typedef struct {
    const char SOH = 0x1; // start of header
    uint32_t seqnum; // sequence number of packet
    const char STX = 0x2; // start of text
    char data; // data
    const char ETX = 0x3; // end of text
    uint8_t checksum; // checksum
} __attribute__((packed)) Packet;

uint8_t computePacketChecksum(uint32_t seqnum, char data);
uint32_t getFileSize(Packet &p);

void makePacket(Packet &p, uint32_t sequenceNumber, char data);
void makeFileSizePacket(Packet &p, uint32_t size);
void makeStartFilePacket(Packet &p);
void makeEndFilePacket(Packet &p);

bool verifyPacket(Packet &p);
bool verifyFileSizePacket(Packet &p, uint32_t size);
bool verifyStartFilePacket(Packet &p);
bool verifyEndFile(Packet &p);