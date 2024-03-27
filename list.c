#include "list.h"

// 遍历客户(1.单纯遍历show，2.登录时，查看是否有已经添加（id唯一）find_id 3.注册时，转发时，查套接字 find_sock)
node *kl_show_fri(node *head, char *flag, int target) 
{
	struct list_head *pos; // 遍历的指针
	node *get_node; // 存储大结构体地址

	if(strcmp(flag, "show") == 0)
	{
		// 表头
		printf("当前已注册用户信息如下：\n");
		printf("用户id\t\t 用户姓名\t 用户密码\t 用户状态\t 用户ip\t 用户端口\t 用户套接字\n");

		list_for_each(pos, &head->list)
		{
			// 通过小结构体地址，获得大结构体地址
			get_node = list_entry(pos, node, list);
			printf("%d\t %s\t\t %s\t %s\t %s\t %hu\t %d\n", get_node->data.id, //id
									get_node->data.name, 	// 用户名
									get_node->data.pwd, // 密码
									(get_node->data.sign==ON_LINE)?"在线":"离线", // 用户状态
									inet_ntoa(get_node->data.c_addr.sin_addr), // ip
									ntohs(get_node->data.c_addr.sin_port),
									get_node->data.con_fd);
		}
		printf("\n");
		return NULL;
	}
	else if(strcmp(flag, "find_id") == 0) // 查id
	{ 
		list_for_each(pos, &head->list)
		{
			get_node = list_entry(pos, node, list);
			if(get_node->data.id == target)
			{
				// printf("该用户已经是好友！\n");
				return get_node; // 返回该用户信息
			}
		}
		return NULL; // 没有加过好友
	}
	else if(strcmp(flag, "find_sock") == 0) // 查套接字
	{
		list_for_each(pos, &head->list)
		{
			get_node = list_entry(pos, node, list);
			if(get_node->data.con_fd == target)
			{
				printf("找到对应套接字！\n");
				return get_node; // 返回该用户信息
			}
		}
		return NULL; // 没有找到对应的套接字
	}
	
}

//初始化
node *kl_init(){
    node *p=malloc(sizeof(node));   
    if(p==NULL){
        perror("malloc error!;");
        return NULL;
    }
    memset(&p->data,0,sizeof(p->data));
    INIT_LIST_HEAD(&p->list);
    return p;
}

//尾插
void kl_add(usr_log_msg new_data,node *head){
    node *p=kl_init();
    p->data=new_data;
    list_add_tail(&p->list,&head->list);
}



// int main(){ 
    // node *head=kl_init();
    // usr_log_msg *data;
    // strcpy(data.name,"cm");
    // kl_add(data,head);
    // printf("%s\n",head->list.prev->data.name);

    // return 0;
// }