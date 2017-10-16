#include <stdio.h>
#include <stdlib.h>

char* filename;
int win_size;
int buf_size;
int port;

void validate(int count, char** &arguments)
{
	if(count != 5)
	{
		printf("Recvfile accept 4 arguments.\n");
		printf("Usage : ./recvfile <filename> <windowsize> <buffersize> <port>\n");
		exit(1);
	}
}

int main(int argc, char** argv)
{
	
	return 0;
}