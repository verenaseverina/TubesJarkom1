#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "receiver_window.h"
#include "receiver_socket.h"
#include "file.h"

/* Global variables */

char filename[256];
unsigned int win_size;
unsigned int buf_size;
unsigned int port;

int sock_fd; // socket file descriptor
struct sockaddr_in server_addr, client_addr; // server and client socket address
socklen_t client_len; // client length

recvWindow window; // receiver window
char* file_buffer; // file buffer

/* Program */

void validate(int count, char** &arg)
{
	// Check argument count
	if(count != 5)
	{
		printf("Recvfile accept 4 arguments.\n");
		printf("Usage : ./recvfile <filename> <windowsize> <buffersize> <port>\n");
		exit(1);
	}

	// Check if windowsize and buffersize is non-negative integer
	int win_size_valid = sscanf(arg[2],"%u",&win_size);
	int buf_size_valid = sscanf(arg[3],"%u",&buf_size);

	// Check port format and port number
	int port_valid = sscanf(arg[4],"%u",&port);

	// If invalid, exit and print error messages	
	if(!(win_size_valid && win_size > 0)) { printf("Invalid windowsize.\n"); exit(1); }
	if(!(buf_size_valid && buf_size > 0)) { printf("Invalid buffersize.\n"); exit(1); }
	if(!(port_valid && port<=65535)) { printf("Invalid destination port.\n"); exit(1); }

	sscanf(arg[1],"%s",filename);
	if(win_size >= 256) win_size = 256;
}

void recv_data()
{
	Packet packet;
	Packet* _packet;
	Ack ack;
	Ack* _ack;
	uint32_t size;

	client_len = sizeof(client_addr);
	receiverMakeWindow(window, win_size);

	while(true)
	{
		recvfrom(sock_fd, _packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
		makePacket(packet, _packet);

		if(verifyFileSizePacket(packet, size))
		{
			size = getFileSize(packet);
			makeFileSizeAck(ack, size);
			_ack = &ack;
			sendto(sock_fd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
		}
		else if(verifyStartFilePacket(packet))
		{
			makeStartFileAck(ack);
			_ack = &ack;
			sendto(sock_fd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
			break;
		}
	}

	while(window.LFR <= size)
	{
		receiverReceivePacket(window, sock_fd, client_addr, client_len, file_buffer);
	}

	recvfrom(sock_fd, _packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
	makePacket(packet, _packet);

	while(true)
	{
		if(verifyEndFilePacket(packet))
		{
			makeEndFileAck(ack);
			_ack = &ack;
			sendto(sock_fd, _ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
			break;
		}
	}

	write_file(filename, size, file_buffer);
	printf("File received.\n");
}

int main(int argc, char** argv)
{
	validate(argc, argv);
	
	open_receiver(sock_fd);
	setup_receiver(server_addr, port);
	bind_socket(sock_fd, server_addr);
	
	recv_data();

	return 0;
}