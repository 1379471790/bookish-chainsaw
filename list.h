#ifndef _LIST_H
#define _LIST_H

#include "kernel_list.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <signal.h>
#include <pthread.h>
#include "./libs/sqlite3.h"

#define APPLY   4	//申请加好友
#define LOGIN 	3  	//登录
#define REGIST 	2  	// 注册
#define ON_LINE 1 	// 在线
#define OFF_LINE 0 	// 离线

typedef struct login
{
    int id; // 用户id号（由服务器分配,8位）
	char name[20]; // 姓名
	char pwd[20]; // 密码
	short sign; // 状态
	struct sockaddr_in c_addr;
	int con_fd;
	sqlite3 *db; // 数据库句柄
}usr_log_msg;


typedef struct node
{
    struct list_head list;
    struct login data;
}node;

node *client_on_list;
node *client_all_list;


node *kl_show_fri(node *head, char *flag, int target);
node *kl_init();
void kl_add(usr_log_msg new_data,node *head);
    
#endif