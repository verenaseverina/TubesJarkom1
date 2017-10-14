#ifndef _MODEL_H_
#define _MODEL_H_

typedef unsigned char Byte;

typedef struct MESSG
{
    Byte soh;
    unsigned int seqnum;
    unsigned int stx;
    unsigned int etx;
    Byte checksum;
    Byte *data;
} MESSG;
#endif