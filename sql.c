#include "sql.h"

#include "./libs/sqlite3.h"

// 数据库的回调函数
int callback(void *p, int num_of_col, char **text, char **name)
{
	char send_buf[1024] = {0};
	if(p!=NULL) // 结果为好友列表信息
	{
		int con_fd = *((int *)p);
		// 每一个i就对应一个好友，把数据要同步给用户，通过指针p来操作，p为客户端的套接字
		sprintf(send_buf, "%s %s %s", text[0], text[1], text[2]);
		send(con_fd, send_buf, strlen(send_buf), 0);
		printf("=============================================\n");
		usleep(300000);
	}
	else if(p == NULL) // 结果为客户端的信息,把客户端信息上链表
	{
		usr_log_msg client; // 定义存储用户信息的结构体变量
		// 4个内容 id, 用户名，密码，标识
		client.id = atoi(text[0]);
		strcpy(client.name,  text[1]);
		strcpy(client.pwd,  text[2]);
		client.sign = 0; // 初始状态：离线
		// 上链表(离线链表)
		kl_add(client, client_all_list);
	}
 	

	return 0;
}

//用户数据库创建
sqlite3 *usr_lib(int id){
    char str[100]={0};
    sqlite3 *db=NULL;
    //创建用户数据库
    sprintf(str,"db/%d.db",id);
    sqlite3_open(str,&db);
    return db;
}

//创建表存储好友和数据
int create_table(sqlite3 *db, char *info){
    char sql[128]={0};
    if(strcmp(info,"friend_info")==0){
        sprintf(sql,"create table if not exists '%s'(id integer primary key not null, name text, sign integer);",info);
    }else if(strcmp(info,"client_info")==0){
        sprintf(sql,"create table if not exists '%s'(id integer primary key not null, name text, pwd text, sign integer);",info);
    }else
	{
		printf("表名不正确\n");
		return -1;
	}
    int ret=sqlite3_exec(db,sql,NULL,NULL,NULL);
    if(ret!=0){
        perror("table create error!\n");
        return -1;
    }
}

// 数据库操作函数集合
/*
db:数据库句柄
data：数据
flag：选中操作模式：insert-1， delete-2，update-3，find-4
con_fd:客户端的套接字
info:选择是操作客户端数据(client_info)，还是好友数据(friend_info)
*/
void sql_cmd(sqlite3 *db,node *data,int flag,int con_fd,char *info){
    char sql[100]={0};

    if(strcmp(info,"friend_info")==0){
        if(flag==INSERT){           //新增好友
            sprintf(sql,"insert into friend_info values('%d','%s','%d');",data->data.id, data->data.name, data->data.sign);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==DELETE){    //删除好友
            sprintf(sql,"delete from friend_info where id='%d';",data->data.id);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==UPDATE){    //更改好友信息
            sprintf(sql,"updata friend_info set name='%s',sign='%d' where id='%d';",data->data.name, data->data.sign, data->data.id);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==FIND){      //查找好友
            
        }
    }else if(strcmp(info,"client_info")==0){
        if(flag==INSERT){           //新增用户
            sprintf(sql,"insert into client_info values('%d','%s','%s','%d');",data->data.id, data->data.name,data->data.pwd, data->data.sign);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==DELETE){     //删除用户
            sprintf(sql,"delete from client_info where id='%d';",data->data.id);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==UPDATE){     //更改用户信息
            sprintf(sql,"updata client_info set name='%s',pwd='%s', sign='%d' where id='%d';",data->data.name,data->data.pwd, data->data.sign, data->data.id);        
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }else if(flag==FIND){       //查找用户
            sprintf(sql, "select * from client_info;");
			sqlite3_exec(db, sql, callback, NULL, NULL);
        }
    }

    // int main(){

    // }
}
