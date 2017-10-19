#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "sender_socket.h"

void open_sender(int &sock_fd)
{
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sock_fd < 0)
	{
		perror("Error opening socket");
		exit(1);
	}

	struct timeval timeout;      
    timeout.tv_sec = 0;
    timeout.tv_usec = 300000; // timeout = 300 ms

	setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)); // set receive timeout to 10 ms
}

void setup_sender(struct sockaddr_in &server_addr, unsigned long ip, unsigned int port_num)
{
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(ip);
	server_addr.sin_port = htons(port_num);
}