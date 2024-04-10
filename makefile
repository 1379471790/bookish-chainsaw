main:network/tcp.c list.c  sql.c libs/sqlite3.c
	gcc -o main main.c network/tcp.c list.c  sql.c libs/sqlite3.c -lpthread -ldl

test:network/tcp.c usr.c list.c
	gcc -o test test.c network/tcp.c usr.c list.c -lpthread

arm:network/tcp.c usr.c
	arm-linux-gcc -o arm test.c network/tcp.c usr.c

shell:
	gcc  shell.c -o shell 

send1:
	scp main root@8.136.117.209:/root/project
send2:
	scp test root1@192.168.247.130:/root/test

clean:
	rm main





#	  gcc -o main main.c network/tcp.c list.c  sql.c libs/sqlite3.c -lpthread -ldl