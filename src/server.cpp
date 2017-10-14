#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string.h>

int main(int argc, char* argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	/* Setting up socket */

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 4444;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/* Bind */

	if(bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}

	clilen = sizeof(cli_addr);

	while(1)
	{
		n = recvfrom(sockfd, buffer, strlen(buffer), 0, (struct sockaddr* ) &cli_addr, &clilen);
		printf("%s\n",buffer);
	}

	return 0;
}
