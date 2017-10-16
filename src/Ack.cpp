#include "Ack.h"

Ack makeAck(uint32_t nextSeqNum, uint8_t advWindowSize){
	//create ack	
	Ack ack;
	ack.next_seqnum = nextSeqNum;
	ack.adv_windsize = advWindowSize;
	
	//count checksum
	ack.checksum = ((uint8_t)ack.ACK + (uint8_t)ack.next_seqnum + ack.adv_windsize)%256;
	return ack;
}