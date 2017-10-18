#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <pthread.h>

#include "sender_window.h"
#include "sender_socket.h"
#include "file.h"

/* Global variables */

char filename[256];
unsigned int win_size;
unsigned int buf_size;
unsigned long dest_ip;
unsigned int dest_port;

int socket_fd; // socket file descriptor
struct sockaddr_in server_addr; // server socket address

sendWindow window; // sender window
char* file_buffer; // file buffer

// pthread condition variables and mutex
pthread_cond_t packet_ack;
pthread_cond_t file_size_ack;
pthread_cond_t start_file_ack;
pthread_cond_t end_file_ack;

pthread_mutex_t mutex;

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
	int win_size_valid = sscanf(arg[2],"%u",&win_size);
	int buf_size_valid = sscanf(arg[3],"%u",&buf_size);

	// Check IP format and IP number
	unsigned int ip[4];
	char extra;
	int ip_valid = sscanf(arg[4],"%u.%u.%u.%u%c",&ip[0],&ip[1],&ip[2],&ip[3],&extra);

	// Check port format and port number
	int port_valid = sscanf(arg[5],"%u",&dest_port);

	// If invalid, exit and print error messages
	if(!(win_size_valid && win_size > 0)) { printf("Invalid windowsize.\n"); exit(1); }
	if(!(buf_size_valid && buf_size > 0)) { printf("Invalid buffersize.\n"); exit(1); }
	if(!(ip_valid == 4 && ip[0]<256 && ip[1]<256 && ip[2]<256 && ip[3]<256)) { printf("Invalid destination IP.\n"); exit(1); }
	if(!(port_valid && dest_port<=65535)) { printf("Invalid destination port.\n"); exit(1); }

	sscanf(arg[1],"%s",filename);
	dest_ip = ((((ip[0] << 8) + ip[1]) << 8 + ip[2]) << 8) + ip[3];
}

void* sendPacket()
{
	FILE* file;
	long size; // file size

	read_file(filename, size, file_buffer);

	Packet packet;
	Packet *_packet;

	// send file size
	makeFileSizePacket(packet, size);
	*_packet = packet;
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&file_size_ack);
	pthread_mutex_unlock(&mutex);

	makeStartFilePacket(packet);
	*_packet = packet;
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&start_file_ack);
	pthread_mutex_unlock(&mutex);

	while(true)
	{
		if(window.recv_adv_windsize != 0) // if adv wind size of receiver is not zero
		{
			if(window.current_size != window.max_size)
			{
				if(window.LFS <= size) // if seqnum <= size
				{
					sendSendPacket(window, sockfd, server_addr, window.LFS, file_buffer[window.LFS - 1]);
				}
				else // all data sent, wait for ack
				{
					pthread_mutex_lock(&mutex);
					pthread_cond_wait(&packet_ack);
					pthread_mutex_unlock(&mutex);
				}
			}
			else // wait for ack
			{
				// paket ilang (?)

				pthread_mutex_lock(&mutex);
				pthread_cond_wait(&packet_ack);
				pthread_mutex_unlock(&mutex);	
			}
		}
		else // wait until timeout
		{
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&packet_ack);
			pthread_mutex_unlock(&mutex);
		}
	}

	makeEndFilePacket(packet);
	*_packet = packet;
	sendto(sockfd, _packet, sizeof(packet), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&end_file_ack);
	pthread_mutex_unlock(&mutex);
}

void* receiveACK()
{
	Ack ack;
	Ack* _ack;

	recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	ack = *_ack;

	if(verifyFileSizeAck(ack))
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&file_size_ack);
		pthread_mutex_unlock(&mutex);
	}

	recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	ack = *_ack;

	if(verifyStartFileAck(ack))
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&start_file_ack);
		pthread_mutex_unlock(&mutex);
	}

	recvfrom(sockfd, _ack, sizeof(ack), 0, (struct sockaddr* ) &server_addr, sizeof(server_addr));
	ack = *_ack;

	if(verifyEndFileAck(ack))
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&end_file_ack);
		pthread_mutex_unlock(&mutex);
	}
}

void* timer()
{

}

void send_data()
{
	//pthread_t thread[3];
	pthread_t thread[2];
	senderMakeWindow(window, win_size);

	// initialize all condition variables and mutex
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&packet_ack, NULL);
	pthread_cond_init (&file_size_ack, NULL);
	pthread_cond_init (&start_file_ack, NULL);
	pthread_cond_init (&end_file_ack, NULL);

	// create 3 threads
	int _thread1 = pthread_create(&thread[0], NULL, sendPacket, NULL); // to send
	int _thread2 = pthread_create(&thread[1], NULL, receiveACK, NULL); // to receive
	int _thread3 = pthread_create(&thread[2], NULL, timer, NULL); // timeout

	// wait for all threads to complete
	int retval;
	pthread_join(&_thread1, (void**) &retval);
	pthread_join(&_thread2, (void**) &retval);
	pthread_join(&_thread3, (void**) &retval);

	// clean up
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&packet_ack);
	pthread_cond_destroy(&file_size_ack);
	pthread_cond_destroy(&start_file_ack);
	pthread_cond_destroy(&end_file_ack);

	free(file_buffer);

	printf("File sent.\n");
}

int main(int argc, char** argv)
{
	validate(argc, argv);

	open_sender(socket_fd);
	setup_sender(server_addr, dest_ip, dest_port);

	send_data();

	return 0;
}