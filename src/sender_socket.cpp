#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sender_socket.h"

void open(int &socket)
{
    socket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket < 0)
	{
		perror("Error opening socket");
		exit(1);
	}
}

void setup(struct sockaddr_in &server_addr, struct hostent* &server, int port_num)
{
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port_num);
}