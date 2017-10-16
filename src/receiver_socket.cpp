#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "receiver_socket.h"

void open(int &socket)
{
    socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socket < 0)
    {
        perror("Error opening socket");
        exit(1);
    }
}

void setup(struct sockaddr_in &server_addr, int port_num)
{
    bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_num);
}

void bind_socket(int socket, struct sockaddr* &server_addr)
{
    int bind = bind(socket,(struct sockaddr*) &server_addr, sizeof(server_addr));
    
    if(bind < 0)
    {
        perror("Error bind socket");
        exit(1);
    }
}