#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <pthread.h>

// 有名管道路径
#define FIFO_PATH "/home/gec/my_fifo"


int main(int argc,char const *argv[]){
        
    char flag[10]={0};//操作标识
   
    printf("argc:%d\n",argc);
    printf("argv[0]:%s\n",argv[0]);
    printf("argv[1]:%s\n",argv[1]);
    printf("argv[2]:%s\n",argv[2]);
    printf("argv[3]:%s\n",argv[3]);

	strcpy(flag, argv[4]);

    //获取id与用户名
    int id=atoi(argv[1]);
    char name[20]={0};
	strcpy(name, argv[2]);
            
	pid_t main_pid = atoi(argv[3]); // 主进程的进程号

    int fifo_fd=open(FIFO_PATH,O_RDWR);
        if(fifo_fd==-1){
            perror("fifo create error!\n");
            return -1;
        }

    char wbuf[1000];
    char send_buf[1024];


    union sigval value; // 定义结构体变量
	memset(&value, 0, sizeof(value));
	value.sival_int = id;
    if(strcmp(flag,"add")==0){
        printf("用户%s请求添加您为好友\n",name);
        printf("是否同意? Y/n\n");
        char c;
        scanf("%c\n",&c);while(getchar()!='\n');
        if(c=='Y'||c=='y'){
            printf("同意\n");
            sigqueue(main_pid,10,value);//同意，将信号10传给test
        }else{
            printf("不同意\n");
            sigqueue(main_pid,12,value);
        }

    }
    while (1)
    {
        memset(wbuf,0,1000);
        printf("请输入发送的数据\n");
		scanf("%s", wbuf);

        // memset(send_buf,0,1024);
        // sprintf(send_buf,"chat %d %s",id,wbuf);

        // write(fifo_fd,send_buf,strlen(send_buf));
        if(!strcmp(wbuf,"bye")==0)
            break;
    }
    
    
}