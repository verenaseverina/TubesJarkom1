#include <stdio.h>
#include <stdlib.h>

char* filename;
int win_size;
int buf_size;
char* dest_ip;
int dest_port;

void validate(int count, char** &arguments)
{
	if(count != 6)
	{
		printf("Sendfile accept 5 arguments.\n");
		printf("Usage : ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>\n");
		exit(1);
	}
}

int main(int argc, char** argv)
{
	
	return 0;
}