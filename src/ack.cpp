#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ack.h"

/* Checksum */

uint8_t computeAckChecksum(uint32_t nextSeqNum, uint8_t advWindowSize)
{
	return ((uint8_t)0x6 + (uint8_t)nextSeqNum + (uint8_t)advWindowSize);
}


/* Create ACK */

void makeAck(Ack &ack, uint32_t nextSeqNum, uint8_t advWindowSize)
{
	ack.next_seqnum = nextSeqNum;
	ack.adv_windsize = advWindowSize;
	ack.checksum = computeAckChecksum(nextSeqNum, advWindowSize);
}

void makeFileSizeAck(Ack &ack, uint32_t size)
{
	ack.next_seqnum = 0xFF000000 + size;
	ack.adv_windsize = 0xFF;
	ack.checksum = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
}

void makeStartFileAck(Ack &ack)
{
	ack.next_seqnum = 0xFF000000;
	ack.adv_windsize = 0xFF;
	ack.checksum = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
}

void makeEndFileAck(Ack &ack)
{
	ack.next_seqnum = 0xFFFFFFFF;
	ack.adv_windsize = 0xFF;
	ack.checksum = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
}


/* Verify ACK */

bool verifyAck(Ack &ack)
{
	uint8_t expected = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
	return (ack.checksum == expected);
}

bool verifyFileSizeAck(Ack &ack)
{
	uint8_t expected = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
	return (ack.adv_windsize == 0xFF && ack.checksum == expected);
}

bool verifyStartFileAck(Ack &ack)
{
	uint8_t expected = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
	return (ack.next_seqnum == 0xFF000000 && ack.adv_windsize == 0xFF && ack.checksum == expected);
}

bool verifyEndFileAck(Ack &ack)
{
	uint8_t expected = computeAckChecksum(ack.next_seqnum, ack.adv_windsize);
	return (ack.next_seqnum == 0xFFFFFFFF && ack.adv_windsize == 0xFF && ack.checksum == expected);
}