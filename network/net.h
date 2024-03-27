#ifndef _NET_H
#define _NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



// #include <errno.h>

int get_s_tcp_fd(int port);
int get_client_fd(int *tcp_fd);

#endif