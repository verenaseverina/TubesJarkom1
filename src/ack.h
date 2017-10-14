#pragma once

typedef struct ACK {
    Byte ACK;
    unsigned int next_seqnum;
    Byte adv_windsize;
    Byte checksum;
}