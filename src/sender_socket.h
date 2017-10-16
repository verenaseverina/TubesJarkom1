#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void open(int &socket);
void setup(struct sockaddr_in &server_addr, struct hostent* &server, int port_num);