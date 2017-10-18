#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void open_sender(int &sock_fd);
void setup_sender(struct sockaddr_in &server_addr, unsigned long ip, unsigned int port_num);