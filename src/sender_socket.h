#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void open_sender(int &socket);
void setup_sender(struct sockaddr_in &server_addr, struct hostent* &server, int port_num);