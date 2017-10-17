#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "sender_socket.h"

void open_sender(int &socket_fd)
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket_fd < 0)
	{
		perror("Error opening socket");
		exit(1);
	}
}

void setup_sender(struct sockaddr_in &server_addr, unsigned long ip, unsigned int port_num)
{
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(ip);
	server_addr.sin_port = htons(port_num);
}