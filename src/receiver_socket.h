#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void open_receiver(int &sock_fd);
void setup_receiver(struct sockaddr_in &server_addr, unsigned int port_num);
void bind_socket(int &sock_fd, struct sockaddr_in &server_addr);