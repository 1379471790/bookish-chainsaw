#include "sql.h"
#include "network/net.h"
#include <unistd.h>

#define MAX 100


int main(){

    //创建链表，保存在线用户
    client_on_list=kl_init();
    //创建链表，保存所有用户
    client_all_list=kl_init();
    


    int tcp_fd=get_s_tcp_fd(8848);

    //创建储存所有用户信息的数据库
    sqlite3 *client_db = usr_lib(10000);
    // 创建存储客户端数据的表格
	create_table(client_db, "client_info");
    //将数据库的信息上链表
    sql_cmd(client_db, NULL, FIND, -1, "client_info");
	kl_show_fri(client_all_list, "show", 0); // 遍历链表

    //epoll多路复用
    struct epoll_event events[MAX];          
    int epoll_fd=epoll_create(1);       //创建epoll句柄   
    if(epoll_fd==-1){
        perror("epoll create error!\n");
        return -1;
    }

	//创建服务端节点结构体上树
    struct epoll_event event;
    event.data.fd=tcp_fd;
    event.events = EPOLLIN |EPOLLET| EPOLLRDHUP;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,tcp_fd,&event);




    node *new_usr=kl_init();
	//接收客户端连接
	while(1)
	{
        int num=epoll_wait(epoll_fd,events,MAX,-1);
        if(num<0){
            printf("epoll error\n");
			break;
        }
        //处理所有事件
        for(int i=0;i<num;i++){
            //取出套接字
            int socket_fd=events[i].data.fd;
            //有新客户端连接
            if(socket_fd==tcp_fd){
				// 存储客户端的地址数据和套接字
                usr_log_msg new_client;
				memset(&new_client, 0, sizeof(new_client));
                socklen_t len = sizeof(new_client.c_addr);
                new_client.con_fd=accept(tcp_fd,(struct sockaddr *)&new_client.c_addr,&len);
                if(new_client.con_fd < 0)
				{
					printf("accept error\n");
					return -1;
				}
                printf("连接成功，当前给客户端分配的套接字=%d\n", new_client.con_fd);


                //将新加入的客户端节点结构体上树
                struct epoll_event event;
                event.data.fd=new_client.con_fd;
                event.events = EPOLLIN |EPOLLET| EPOLLRDHUP;
                epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_client.con_fd,&event);
            }else if(events[i].events & EPOLLIN ){  //读取客户端数据****注册*****登录*****
                printf("客户端%d有数据到来,",socket_fd);
                memset(new_usr,0,sizeof(new_usr));
	        	// read(socket_fd, &(new_usr->data), sizeof(*new_usr));
                recv(socket_fd,&(new_usr->data),sizeof(*new_usr),0);
                printf("sign=%d\n",new_usr->data.sign);
                // 用户注册，将数据存入数据库
		        	if(new_usr->data.sign==REGIST){
                    //生成一个随机id
                        srand(time(NULL));
                        new_usr->data.id = rand() % 90000000 + 10000000;
                        new_usr->data.sign=OFF_LINE;
                        //将id发给客户端
                        send(socket_fd,&(new_usr->data.id),sizeof(new_usr->data.id),0);
                        //执行sql语句
                        sql_cmd(client_db,new_usr,INSERT,socket_fd,"client_info");
                        //将新用户加入链表
                        kl_add(new_usr->data,client_all_list);
                // 用户登录    
                    }else if(new_usr->data.sign==LOGIN){
                        //查询
                        node *find=kl_show_fri(client_all_list,"find_id",new_usr->data.id);
                        if(find==NULL){
                            printf("无此用户！\n");
                            send(socket_fd,"nouser",6,0);
                        }else if((find->data.id==new_usr->data.id)&&(strcmp(find->data.pwd,new_usr->data.pwd)==0)){
                            new_usr->data.sign=ON_LINE;
                            new_usr->data.con_fd=socket_fd;
                            printf("又在denglu?\n");
                            send(socket_fd,"right",6,0);
                        }else
                            send(socket_fd,"no_co",6,0);
                        //好友申请
                    }else if(new_usr->data.sign==APPLY){
                        printf("有好友申请0000\n");
                        //查询发送好友申请的用户
    					node *client_now = kl_show_fri(client_all_list, "find_sock", socket_fd); 
                        
                        printf("有好友申请\n");
                        int  id=new_usr->data.id;
                        //查询该id
                        node *apply=kl_show_fri(client_all_list,"find_id",id);
                        if(apply==NULL){
                            printf("无此id\n"); 
                        }else if(apply->data.sign==OFF_LINE){
                            printf("该用户不在线\n");
                        }else if(apply->data.sign==ON_LINE){
                            char buf[100]={0};
                            memset(buf, 0, 1024);
						    sprintf(buf, "add_fri wait %d %s", client_now->data.id, client_now->data.name);
                            send(apply->data.con_fd,buf,strlen(buf),0);
                            printf("以向该用户发送请求\n");
                            printf("%s\n",buf);
                        }
                    }
            }

	    }
    }

	close(tcp_fd);
    
    return 0;
}