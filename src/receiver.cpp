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
	Ack ack;
	uint32_t size;

	client_len = sizeof(client_addr);
	printf("Creating recvWindow... ");
	receiverMakeWindow(window, win_size);
	printf("recvWindow created.\nInital window state : LAF = %d, LFR = %d, max_size = %d\n\n", window.LAF, window.LFR, window.max_size);
	
	recvfrom(sock_fd, &packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
	size = getFileSize(packet);
	printf("Received file size from sender. File size : %d bytes\n", size);
	printf("Checking packet integrity...\n");

	if(verifyFileSizePacket(packet, size))
	{
		printf("Integrity verified.\n\nSending ACK for file size...\n");
		file_buffer = (char*) malloc(sizeof(char) * size);
		makeFileSizeAck(ack, size);
		sendto(sock_fd, &ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
		printf("ACK for file size sent.\n\n");
	}
	else
	{
		printf("Integrity cannot be verified. Exiting now.\n\n");
		exit(1);
	}
	
	printf("Receiving %d bytes as \"%s\"...\n\n", size, filename);
	/*
	recvfrom(sock_fd, &packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
	
	if(verifyStartFilePacket(packet))
	{
		makeStartFileAck(ack);
		sendto(sock_fd, &ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
	}
	*/
	while(window.LFR < size)
	{
		receiverReceivePacket(window, sock_fd, client_addr, client_len, file_buffer, size);
	}
	/*
	recvfrom(sock_fd, &packet, sizeof(packet), 0, (struct sockaddr* ) &client_addr, &client_len);
	
	if(verifyEndFilePacket(packet))
	{
		makeEndFileAck(ack);
		sendto(sock_fd, &ack, sizeof(ack), 0, (struct sockaddr* ) &client_addr, sizeof(client_addr));
	}
	*/

	printf("File received. Writing file to filesystem...\n");

	write_file(filename, size, file_buffer);
	printf("File written.\nRecvfile complete.");
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