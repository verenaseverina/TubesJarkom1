#pragma once

typedef struct {
    char ACK; // ACK
    uint32_t next_seqnum; // next sequence number
    uint8_t adv_windsize; // advertised window size
    uint8_t checksum; // checksum
}
ack;