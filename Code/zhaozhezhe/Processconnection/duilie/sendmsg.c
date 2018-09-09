#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define buf_size 1024
#define pid 222
#define pathname "."

int main(void){

	struct my_msgbuf{
		long msgtype;
		char ctrlstring[buf_size];
	}msgbuffer;

	int msgid;
	int msglen;
	key_t msgkey;

	if((msgkey = ftok(pathname, pid)) == -1){
		printf("ftok error.\n");\
		exit(1);
	}

	if((msgid = msgget(msgkey, IPC_CREAT|0660)) == -1){
		printf("msgget error.\n");
		exit(0);
	}

	msgbuffer.msgtype = 3;

	strcpy(msgbuffer.ctrlstring, "Hello, mssage queue.\n");

	if(msgsnd(msgid, &msgbuffer, sizeof(msgbuffer.ctrlstring), 0) == -1){
		printf("msgsnd error.\n");
		exit(1);
	}

	exit(0);
}
