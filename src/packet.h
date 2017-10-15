#pragma once

typedef struct {
    const char SOH = 0x1; // start of header
    uint32_t seqnum; // sequence number of packet
    const char STX = 0x2; // start of text
    char data; // data
    const char ETX = 0x3; // end of text
    uint8_t checksum; // checksum
} packet;