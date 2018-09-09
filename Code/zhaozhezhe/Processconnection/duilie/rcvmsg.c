#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
		printf("ftok error.\n");
		exit(1);
	}

	if((msgid = msgget(msgkey, IPC_CREAT|0660)) == -1){
		printf("msgget error.\n");
		exit(1);
	}

	if(msgrcv(msgid, &msgbuffer, sizeof(msgbuffer.ctrlstring), 3, 0) == -1){
		printf("msgrcv error.\n");
		exit(1);
	}
	
	printf("Get mssage %s \n", msgbuffer.ctrlstring);

	return 0;
}
