#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


/* Constants */

#define BUFFER_SIZE 256

#define OPEN_ERR "open"
#define BIND_ERR "host"


/* Global variables */

int socket_fd; // socket file descriptor, 
int port_num; // port number

struct sockaddr_in server_addr, client_addr; // server and client socket address
socklen_t client_len; // client server length
char buffer[BUFFER_SIZE]; // buffer

char* file_buffer; // buffer for write file
long file_size = 0; // file size in bytes

int _recv;


/* Subprograms */

void error(const char* type)
{
	// Print error message
	if(strcmp(type, "open") == 0) perror("Socket open error");
	else if(strcmp(type, "bind") == 0) perror("Socket bind error");

	exit(1);
}

void setup()
{
	// Open socket
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket_fd < 0) error(OPEN_ERR);

	// Set server address and port number
	bzero((char *) &server_addr, sizeof(server_addr));
	port_num = 4444;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_num);

	// Bind socket
	if(bind(socket_fd,(struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) error(BIND_ERR);
}

void recv_data()
{
	// Receive and print message
	client_len = sizeof(client_addr);
	file_buffer = (char*) malloc(sizeof(char) * 1024); // allocate buffer for file (still hardcoded)
	int cnt = 0; // counter

	while(cnt < 3) // how many times we need to receive the file (still hardcoded)
	{
		_recv = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr* ) &client_addr, &client_len); // receive data
		
		if(strlen(buffer) >= BUFFER_SIZE)
		{
			bcopy(buffer, file_buffer+(BUFFER_SIZE * cnt), BUFFER_SIZE * sizeof(char));
			file_size += (sizeof(char) * BUFFER_SIZE); 
		}
		else
		{
			bcopy(buffer, file_buffer+(BUFFER_SIZE * cnt), strlen(buffer) * sizeof(char));  // copy non-null bytes to buffer
			file_size += (sizeof(char) * strlen(buffer)); // compute file size, use strlen to ignore trailing NULL
		}

		cnt++; // increment counter to fill the next BUFFER_SIZE bytes
	}

	file_buffer[file_size] = '\0'; // ensure buffer is ended with NULL
}

void write_file()
{
	FILE* file;
	const char* filename = "lorem.txt"; // file path relative to root folder

	file = fopen(filename,"wb"); // open in binary mode

	fwrite(file_buffer, 1, file_size, file); // write file
	fclose(file);
	free(file_buffer); // free buffer

	printf("File received.\n"); // notify user
}


/* Main */

int main(int argc, char* argv[])
{
	setup();
	recv_data();
	write_file();
	
	return 0;
}