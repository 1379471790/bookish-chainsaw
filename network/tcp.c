#include "net.h"


//获取服务端套接字
int get_s_tcp_fd(int port){
    // 1.创建tcp套接字
	int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_fd == -1)
	{
		perror("socket tcp error\n");
		return -1;
	}
	// 设置地址可重用
	int on=1;
	setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	// 2.准备自身的地址结构体
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // 地址族
	server_addr.sin_port = htons(port); // 端口
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip
	// 3.绑定
	bind(tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	// 4.监听
	listen(tcp_fd, 1);
	// 5.返回监听的套接字
	return tcp_fd;
}

//客户端套接字,连接服务端
int get_client_fd(int *tcp_fd){
    // 1.创建tcp套接字
	*tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(*tcp_fd == -1)
	{
		perror("socket tcp error\n");
		return -1;
	}
	//自身地址结构体
	struct sockaddr_in self_addr;
	memset(&self_addr, 0, sizeof(self_addr));
	self_addr.sin_family = AF_INET; //
	self_addr.sin_port = htons(8848); // 端口
	inet_pton(AF_INET, "192.168.13.141", (struct sockaddr *)&self_addr.sin_addr);
	bind(*tcp_fd, (struct sockaddr *)&self_addr, sizeof(self_addr));

	//服务端地址结构体
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // 地址族
	server_addr.sin_port = htons(8848); // 端口
	inet_pton(AF_INET, "8.136.117.209", (struct sockaddr *)&server_addr.sin_addr);

	int ret=connect(*tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret == -1)
		return -1;
	// 6.返回连接的套接字
	return *tcp_fd;
}


// int main(){
// 	int tcp_fd;
// 	get_client_fd(&tcp_fd);

// 	write(tcp_fd,"cmcmcm",6);

// 	return 0;
// }
