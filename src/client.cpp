#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/* Global variables */

int socket_fd; // socket file descriptor
int port_num; // port number

struct sockaddr_in server_addr; // server socket address
struct hostent *server; // host information
char buffer[256]; // buffer
int _send;

const char* open_err = "open";
const char* host_err = "host";

/* Subprograms */

void error(const char* type)
{
	if(strcmp(type, "open") == 0) perror("Socket open error");
	else if(strcmp(type, "host") == 0) perror("Socket host error");

	exit(1);
}

void setup()
{
	// Open socket
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket_fd < 0) error(open_err);

	// Set server address
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port_num);
}

void send_data()
{
	printf("Please enter the message: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);

	_send = sendto(socket_fd, buffer, 256, 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
}

/* Main */

int main(int argc, char *argv[])
{
	port_num = atoi(argv[2]); // convert port number to integer
	server = gethostbyname(argv[1]); // get host from name

	if(server == NULL) error(host_err);

	setup();
	send_data();

	return 0;
}
