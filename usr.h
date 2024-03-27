#ifndef _USR_H
#define _USR_H

#include "list.h"

node *head_fri;

void *routine(void *arg);
int add_friend(int tcp_fd);
int register_login(int tcp_fd);

#endif