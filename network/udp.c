#include "net.h"


//获取服务端套接字
int get_s_udp_fd(int port){
    int udp_fd=socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_fd == -1)
	{
		perror("socket error\n");
		return -1;
	}
    struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(udp_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    return udp_fd;
}

// 获取套接字和服务端的地址数据
struct sockaddr_in get_udp_fd_saddr(int *udp_fd, int port, char *s_addr)
{
	// 创建udp套接字
	*udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(*udp_fd == -1)
	{
		perror("socket udp error\n");
		exit(0);
	}
	// 确定服务端地址结构体
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, s_addr, &server_addr.sin_addr);

	return server_addr;
}