#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "receiver_window.h"
#include "receiver_socket.h"

/* Global variables */

char filename[256];
unsigned int win_size;
unsigned int buf_size;
unsigned int port;

int socket_fd; // socket file descriptor
struct sockaddr_in server_addr, client_addr; // server and client socket address
socklen_t client_len; // client server length

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
}

int main(int argc, char** argv)
{
	validate(argc, argv);
	
	open_receiver(socket_fd);
	setup_receiver(server_addr, port);
	bind_socket(socket_fd, server_addr);
	
	/*
	char buffer[256];

	client_len = sizeof(client_addr);
	bzero(buffer,256);
	int _recv = recvfrom(socket_fd, buffer, 256, 0, (struct sockaddr* ) &client_addr, &client_len);

	printf("%s\n",buffer);
	*/
	
	recvWindow window = makeWindow(win_size);

	return 0;
}