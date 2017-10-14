#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Global Variable
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;

void configureSetting(int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
}
int main (int argc, char* argv[]) {
	if (argc != 5) {
		cout << "Usage : ./recvfile <filename> <windowsize> <buffersize> <port>" << endl;
	} else {
		int udpSocket, portNum;
		portNum = atoi(argv[1]);
	  	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	  	configureSetting(portNum);
		bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
		
		addr_size = sizeof serverStorage;
	}
	return 0;
}