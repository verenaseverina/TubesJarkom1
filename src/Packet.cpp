#include "Packet.h"

Packet makePacket(uint32_t sequenceNumber, char dataPayload){
	//create packet	
	Packet p;
	p.seqnum = sequenceNumber;
	p.data = dataPayload;

	//count checksum
	p.chekcksum = ((uint8_t)p.SOH + (uint8_t)p.seqnum + (uint8_t)p.STX + (uint8_t)p.data + (uint8_t)p.ETX)%256;
	return p;
}

