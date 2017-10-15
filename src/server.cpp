#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/* Global variables */

int socket_fd; // socket file descriptor, 
int port_num; // port number

struct sockaddr_in server_addr, client_addr; // server and client socket address
socklen_t client_len; // client server length

char buffer[256]; // buffer
int _recv;

const char* open_err = "open";
const char* bind_err = "bind";

/* Subprograms */

void error(const char* type)
{
	if(strcmp(type, "open") == 0) perror("Socket open error");
	else if(strcmp(type, "bind") == 0) perror("Socket bind error");

	exit(1);
}

void setup()
{
	// Open socket
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket_fd < 0) error(open_err);

	// Set server address and port number
	bzero((char *) &server_addr, sizeof(server_addr));
	port_num = 4444;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_num);

	// Bind socket
	if(bind(socket_fd,(struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) error(bind_err);
}

void recv()
{
	// Receive and print message
	client_len = sizeof(client_addr);
	
	_recv = recvfrom(socket_fd, buffer, 256, 0, (struct sockaddr* ) &client_addr, &client_len);
	printf("%s",buffer);
}

/* Main */

int main(int argc, char* argv[])
{
	setup();
	recv();
	
	return 0;
}