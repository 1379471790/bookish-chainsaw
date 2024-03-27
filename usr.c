#include "usr.h"
#include "list.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

void *routine(void *arg){

}


//注册登录----客户端
int register_login(int tcp_fd){
    char msg[100]={0};     //接收客户端发过来的信息
    usr_log_msg new_usr;   //用户结构体
    int flag;

    while (1){
        printf("1——————登录\n");
        printf("2——————注册\n");
        scanf("%d",&flag);while(getchar()!='\n');
        //**********注册**********
            if(flag==2){
                while(1){
                    printf("请输入用户名，长度不超过20字符\n");
                    scanf("%s",new_usr.name);while(getchar()!='\n');
                    printf("请输入密码，长度不超过20字符\n");
                    scanf("%s",new_usr.pwd);while(getchar()!='\n');
                    if(strlen(new_usr.pwd)>20||strlen(new_usr.name)>20){
                        printf("输入字符过长，重新输入\n");
                        continue;
                    }
                    else
                        break;
                }
                new_usr.sign=REGIST;
                //将用户数据发送给客户端
                send(tcp_fd,&new_usr,sizeof(new_usr),0); 
                //接收id号
                recv(tcp_fd,&(new_usr.id),sizeof(new_usr.id),0);
                printf("注册成功，用户%s的id为：%d\n",new_usr.name,new_usr.id);    
            }else if(flag==1){
        //**********登录**********
                while(1){
                    printf("请输入用户ID:\n");
                    scanf("%d",&new_usr.id);while(getchar()!='\n');
                    printf("请输入密码:\n");
                    scanf("%s",new_usr.pwd);while(getchar()!='\n');
                    if(strlen(new_usr.pwd)>20||strlen(new_usr.name)>20){
                        printf("输入字符过长:\n");
                        continue;
                    }
                    else
                        break;
                }
                 new_usr.sign=LOGIN;
                //将用户数据发送给客户端
                send(tcp_fd,&new_usr,sizeof(new_usr),0); 
                recv(tcp_fd,msg,10,0);
                if(strcmp(msg,"nouser")==0){
                    printf("用户名不存在,请重试\n");
                    continue;
                }else if(strcmp(msg,"right")==0){
                    printf("账户%s登陆成功\n",new_usr.name);
                    break;
                }else if(strcmp(msg,"no_co")==0){
                    printf("用户名或密码输入错误,请重试\n");
                    continue;
                }
        }
}
    return 0;
}

//添加好友----客户端
int add_friend(int tcp_fd){
    int id;
    printf("请输入该用户的id号\n");
	scanf("%d", &id);while(getchar()!='\n');
    node *ret = kl_show_fri(head_fri, "find_id", id);
    
    // usr_log_msg new;
    // new.id=54091939;
    // strcpy(new.name,"zxc");
    // kl_add(new,head_fri);
    
	if(ret!=NULL)
	{
		printf("该用户已经是好友\n");
        printf("%d\n",ret->data.id);
		return  -1;
	}else{
        ret=kl_init();
        ret->data.id=id;
        ret->data.sign=APPLY;
        //申请好友信息发给服务器
	    send(tcp_fd,&(ret->data), sizeof(*ret), 0);
        // write(tcp_fd,&(ret->data),sizeof(*ret));
        printf("1111\n");
    }
    

    return 0;
}   


