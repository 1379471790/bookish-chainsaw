#ifndef _SQL_H
#define _SQL_H

#include "libs/sqlite3.h"
#include "list.h"


#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define FIND   4


sqlite3 *usr_lib(int id);
int create_table(sqlite3 *db, char *info);
void sql_cmd(sqlite3 *db, node *data ,int flag, int con_fd, char *info);


#endif