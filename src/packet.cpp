#include "packet.h"

/* Checksum */

uint8_t computePacketChecksum(uint32_t seqnum, char data)
{
	return ((uint8_t)0x1 + (uint8_t)seqnum + (uint8_t)0x2 + (uint8_t)data + (uint8_t)0x3);
}


/* Extractor */

uint32_t getFileSize(Packet &p)
{
	return p.seqnum - 0xFF000000;
}


/* Create Packet*/

void makePacket(Packet &p, uint32_t sequenceNumber, char data)
{
	p.SOH = 0x1;
	p.STX = 0x2;
	p.ETX = 0x3;
	p.seqnum = sequenceNumber;
	p.data = data;
	p.checksum = computePacketChecksum(sequenceNumber, data);
}

void makePacket(Packet &p, Packet* &_p)
{
	p.SOH = _p->SOH;
	p.STX = _p->STX;
	p.ETX = _p->ETX;
	p.seqnum = _p->seqnum;
	p.data = _p->data;
	p.checksum = _p->checksum;
}

void makeFileSizePacket(Packet &p, uint32_t size)
{
	p.SOH = 0x1;
	p.STX = 0x2;
	p.ETX = 0x3;
	p.seqnum = 0xFF000000 + size;
	p.data = (char) 0xFF;
	p.checksum = computePacketChecksum(p.seqnum, p.data);
}

void makeStartFilePacket(Packet &p)
{
	p.SOH = 0x1;
	p.STX = 0x2;
	p.ETX = 0x3;
	p.seqnum = 0xFF000000;
	p.data = (char) 0xFF;
	p.checksum = computePacketChecksum(p.seqnum, p.data);
}

void makeEndFilePacket(Packet &p)
{
	p.SOH = 0x1;
	p.STX = 0x2;
	p.ETX = 0x3;
	p.seqnum = 0xFFFFFFFF;
	p.data = (char) 0xFF;
	p.checksum = computePacketChecksum(p.seqnum, p.data);
}


/* Verify Packet */

bool verifyPacket(Packet &p)
{
	uint8_t expected = computePacketChecksum(p.seqnum, p.data);
	return (p.checksum == expected);
}

bool verifyFileSizePacket(Packet &p, uint32_t size)
{
	uint8_t expected = computePacketChecksum(p.seqnum, p.data);
	return (p.seqnum == 0xFF000000 + size && p.data == (char) 0xFF && p.checksum == expected);
}

bool verifyStartFilePacket(Packet &p)
{
	uint8_t expected = computePacketChecksum(p.seqnum, p.data);
	return (p.seqnum == 0xFF000000 && p.data == (char) 0xFF && p.checksum == expected);
}

bool verifyEndFilePacket(Packet &p)
{
	uint8_t expected = computePacketChecksum(p.seqnum, p.data);
	return (p.seqnum == 0xFFFFFFFF && p.data == (char) 0xFF && p.checksum == expected);
}