#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Packet.h"
#include "Ack.h"

/* Constants */

#define BUFFER_SIZE 256

#define OPEN_ERR "open"
#define HOST_ERR "host"
#define FILE_OPEN_ERR "file_open"
#define FILE_READ_ERR "file_read"

/* Global variables */

int socket_fd; // socket file descriptor
int port_num; // port number

struct sockaddr_in server_addr; // server socket address
struct hostent *server; // host information
char buffer[BUFFER_SIZE]; // buffer

char* file_buffer; // buffer for read file
long file_size; // file size in bytes

int _send;


/* Subprograms */

void error(const char* type)
{
	// Print error message
	if(strcmp(type, "open") == 0) perror("Socket open error");
	else if(strcmp(type, "host") == 0) perror("Socket host error");
	else if(strcmp(type, "file_open") == 0) perror("File open error");
	else if(strcmp(type, "file_read") == 0) perror("File read error");

	exit(1);
}

void setup()
{
	// Open socket
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(socket_fd < 0) error(OPEN_ERR);

	// Set server address
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(port_num);
}

void read_file()
{
	FILE* file;
	const char* filename = "data/lorem.txt"; // file path relative to root folder

	file = fopen(filename,"rb"); // open in binary mode

	if(file == NULL) error(FILE_OPEN_ERR);
	else
	{
		fseek(file, 0, SEEK_END);
		file_size = ftell(file); // get file size
		rewind(file);

		file_buffer = (char*) malloc(sizeof(char) * file_size); // allocate buffer for file

		size_t read_size = fread(file_buffer, 1, file_size, file); // read file
		if(read_size != file_size) error(FILE_READ_ERR); // check if read file size equal to file size
		
		fclose(file);
	}
}

void send_data()
{
	int count = (int) ceil((double)file_size / BUFFER_SIZE); // count how many times we need to send file
	int last = (int) file_size % BUFFER_SIZE; // the last buffer may not have size of BUFFER_SIZE, compute to prevent double free or corruption

	if(last == 0 && file_size != 0) last = BUFFER_SIZE; // handle if last buffer is 256 bytes

	for(int i=0;i<count;i++)
	{
		bzero(buffer, BUFFER_SIZE); // initialize all with zero
		
		if(i != count-1) bcopy(file_buffer+(BUFFER_SIZE * i), buffer, BUFFER_SIZE * sizeof(char)); // if not last buffer, copy BUFFER_SIZE bytes
		else bcopy(file_buffer+(BUFFER_SIZE * i), buffer, last * sizeof(char)); // else copy the remaining only

		_send = sendto(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr* ) &server_addr, sizeof(server_addr)); // send data
	}
	
	free(file_buffer); // free buffer
	printf("File sent.\n"); // notify user
}


/* Main */

int main(int argc, char *argv[])
{
	port_num = atoi(argv[2]); // convert port number to integer
	server = gethostbyname(argv[1]); // get host from name

	if(server == NULL) error(HOST_ERR);

	setup();
	read_file();
	send_data();

	return 0;
}