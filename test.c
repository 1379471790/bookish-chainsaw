#include "network/net.h"
#include "usr.h"


	int tcp_fd;


//捕获信号并获取数据(加好友)
void fun(int sig,siginfo_t *sig_struct, void *sigp){
	usr_log_msg add;
	memset(&add,0,sizeof(add));
	if(sig==10){
		strcpy(add.msg,"add_friend agree");
		add.id=sig_struct->si_int;
		send(tcp_fd,&add,sizeof(add),0);
	}else if(sig==12){
		strcpy(add.msg,"add_friend disagree");
		add.id=sig_struct->si_int;
		send(tcp_fd,&add,sizeof(add),0);
	}
}

int main(){
	head_fri = kl_init(); // 存储好友列表


	get_client_fd(&tcp_fd);
    char buf[100]={0};
	//登录注册
    register_login(tcp_fd);
	// 登录成功后，开启线程
	pthread_t tid;
	pthread_create(&tid, NULL, routine, (void *)&tcp_fd);

	//开启信号捕捉
	struct sigaction my_act;
	my_act.sa_flags=SA_SIGINFO;
	my_act.sa_sigaction = fun; //响应函数
	sigaction(10, &my_act, NULL);
	sigaction(12, &my_act, NULL);

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