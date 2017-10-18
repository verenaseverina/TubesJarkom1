#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "receiver_socket.h"

void open_receiver(int &sock_fd)
{
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sock_fd < 0)
    {
        perror("Error opening socket");
        exit(1);
    }
}

void setup_receiver(struct sockaddr_in &server_addr, unsigned int port_num)
{
    bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_num);
}

void bind_socket(int &sock_fd, struct sockaddr_in &server_addr)
{
    int _bind = bind(sock_fd,(struct sockaddr*) &server_addr, sizeof(server_addr));
    
    if( _bind < 0)
    {
        perror("Error bind socket");
        exit(1);
    }
}