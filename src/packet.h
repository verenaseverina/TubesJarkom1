#pragma once

typedef struct {
    char SOH; // start of header
    uint32_t SeqNum; // sequence number of packet
    char STX; // start of text
    char data; // data
    char ETX; // end of text
    uint8_t checksum; // checksum
} packet;