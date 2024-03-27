#include "network/net.h"
#include "usr.h"

int main(){
	head_fri = kl_init(); // 存储好友信息


	int tcp_fd;
	get_client_fd(&tcp_fd);
    char buf[100]={0};
	//登录注册
    register_login(tcp_fd);
	// 登录成功后，开启线程
	pthread_t tid;
	pthread_create(&tid, NULL, routine, (void *)&tcp_fd);

    while(1){
		// 登录成功后，具体功能
		printf("请选择需要操作的功能\n");
		printf("0--------------添加好友！\n");
		printf("1--------------删除好友！\n");
		printf("2--------------设置状态！\n");
		printf("3--------------发送消息！\n");
		printf("4--------------发送文件！\n");
		printf("5--------------进入群聊！\n");
		printf("6--------------发起群聊！\n");
		int cmd;
		scanf("%d", &cmd);while(getchar()!='\n');	
		switch(cmd){
			case 0: add_friend(tcp_fd);break;
			case 1:break;
			
			
		}
    }
	return 0;
}