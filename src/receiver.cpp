#include <stdio.h>
#include <stdlib.h>

/* Global variables */

char filename[256];
int win_size = 0;
int buf_size = 0;
int port = -1;

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
	int win_size_valid = sscanf(arg[2],"%d",&win_size);
	int buf_size_valid = sscanf(arg[3],"%d",&buf_size);

	// Check port format and port number
	int port_valid = sscanf(arg[4],"%d",&port);

	// If invalid, exit and print error messages	
	if(!(win_size_valid && win_size > 0)) { printf("Invalid windowsize.\n"); exit(1); }
	if(!(buf_size_valid && buf_size > 0)) { printf("Invalid buffersize.\n"); exit(1); }
	if(!(port_valid && port>=0 && port<=65535)) { printf("Invalid destination port.\n"); exit(1); }

	sscanf(arg[1],"%s",filename);
}

int main(int argc, char** argv)
{
	validate(argc, argv);

	return 0;
}