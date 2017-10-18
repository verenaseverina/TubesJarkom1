#include <algorithm>

#include "receiver_window.h"

void receiverMakeWindow(recvWindow &window, uint32_t maxSize)
{
	window.max_size = maxSize;
	window.LAF = maxSize;
	window.LFR = 0;
	window.lastIndex = -1; // no missing packet
	window.bufferCount = 0;

	for(int i=0;i<256;i++) window.receiverBuffer[i] = '\0'; // fill receiver buffer with NULL
}

void receiverReceivePacket(recvWindow &window, int &sockfd, struct sockaddr_in &client_addr, socklen_t &client_len, char* &file_buffer)
{
	Packet packet;
	Packet* _packet;

	recvfrom(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
	makePacket(packet, _packet);
	
	if(verifyPacket(packet)) // check paket
	{
		if(packet.seqnum > window.LFR) // new packet
		{
			char data = packet.data; // store data from packet

			if(window.lastIndex == -1) // if there is no missing packet
			{ 
				file_buffer[packet.seqnum - 1] = data; // directly store to file buffer in corresponding index
				receiverSendACK(window, sockfd, client_addr, packet.seqnum + 1); // send ACK
				window.LAF++;
				window.LFR++;
			}
			else // if there are missing packet
			{
				if(window.max_size - window.bufferCount != 0) // adv wind size is not zero
				{
					if(packet.seqnum == window.LFR + 1) // if the expected packet received
					{
						window.receiverBuffer[0] = data;
						window.bufferCount++; // increment buffer count
						int missing_idx = window.lastIndex + 1; // the next empty index

						for(int i=0;i<=window.lastIndex;i++) // search until lastIndex
						{
							if(window.receiverBuffer[i] == '\0') // find the next missing index
							{
								missing_idx = i;
								break;
							}
						}

						// move data from recv buffer to file buffer
						for(int i=0;i<missing_idx;i++)
						{
							file_buffer[window.LFR + i] = window.receiverBuffer[i];
							window.bufferCount--; // decrement buffer count
							window.receiverBuffer[i] = '\0'; // set to NULL
						}

						if(missing_idx != window.lastIndex + 1) // there are unordered packets again
						{
							for(int i=missing_idx;i<=window.lastIndex;i++)  // move all received unordered packets to front
							{
								window.receiverBuffer[i-missing_idx] = window.receiverBuffer[i];
							}

							window.lastIndex -= missing_idx;
						}
						else
						{
							window.lastIndex = -1; // no unordered packets in buffer
						}
						
						window.LFR += missing_idx;
						window.LAF = window.max_size + window.LFR;
					}
					else
					{
						int store_index = packet.seqnum - window.LFR - 1;  // compute corresponding index in recv buffer

						if(window.receiverBuffer[store_index] == '\0') // if buffer is filled, ignore the packet
						{
							window.lastIndex = std::max(store_index, window.lastIndex); // update lastIndex if necessary
							window.receiverBuffer[store_index] = data; // store to recv buffer first
							window.bufferCount++; // increment bufferCount
						}
						
						receiverSendACK(window, sockfd, client_addr, window.LFR + 1); // next sequence number = LFR + 1 (expected packet)
					}
				}
				else receiverSendACK(window, sockfd, client_addr, window.LAF + 1); // send ACK with 0 adv window size
			}
		}
		else // ACK is lost, but packet is received
		{
			receiverSendACK(window, sockfd, client_addr, window.LFR + 1); // next sequence number = LFR + 1 (expected packet)
		}
	}
}

void receiverSendACK(recvWindow &window, int &sockfd, struct sockaddr_in &client_addr, uint32_t nextSeqNum)
{
	Ack ack;
	Ack* _ack;
	
	makeAck(ack, nextSeqNum, window.max_size - window.bufferCount); // adv window size = maxSize - bufferCount
	_ack = &ack;
	
	sendto(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
}