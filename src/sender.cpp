#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables */

char filename[256];
int win_size = 0;
int buf_size = 0;
char dest_ip[15];
int dest_port = -1;

/* Program */

void validate(int count, char** &arg)
{
	// Check argument count
	if(count != 6)
	{
		printf("Sendfile accept 5 arguments.\n");
		printf("Usage : ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>\n");
		exit(1);
	}

	// Check if windowsize and buffersize is non-negative integer
	int win_size_valid = sscanf(arg[2],"%d",&win_size);
	int buf_size_valid = sscanf(arg[3],"%d",&buf_size);

	// Check IP format and IP number
	int ip[4];
	char extra;
	int ip_valid = sscanf(arg[4],"%d.%d.%d.%d%c",&ip[0],&ip[1],&ip[2],&ip[3],&extra);

	// Check port format and port number
	int port_valid = sscanf(arg[5],"%d",&dest_port);

	// If invalid, exit and print error messages
	if(!(win_size_valid && win_size > 0)) { printf("Invalid windowsize.\n"); exit(1); }
	if(!(buf_size_valid && buf_size > 0)) { printf("Invalid buffersize.\n"); exit(1); }
	if(!(ip_valid == 4 && ip[0]>=0 && ip[0]<256 && ip[1]>=0 && ip[1]<256 && ip[2]>=0 && ip[2]<256 && ip[3]>=0 && ip[3]<256)) { printf("Invalid destination IP.\n"); exit(1); }
	if(!(port_valid && dest_port>=0 && dest_port<=65535)) { printf("Invalid destination port.\n"); exit(1); }

	sscanf(arg[1],"%s",filename);
	sscanf(arg[4],"%s",dest_ip);
}

int main(int argc, char** argv)
{
	validate(argc, argv);

	return 0;
}