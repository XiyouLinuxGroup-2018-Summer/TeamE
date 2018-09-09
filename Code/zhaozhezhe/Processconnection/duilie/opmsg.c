#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define buf_size 1024
#define pid 222
#define pathname "."
void getmsgattr(int msgid, struct msqid_ds msg_info);
int main(void){

	struct my_msgbuf{
	
		long msgtype;
		char ctrlstring[buf_size];
	}msgbuffer;

	int msgid;
	int msglen;
	key_t msgkey;

	struct msqid_ds msq_attr;

	if((msgkey = ftok(pathname, pid)) == -1){
		printf("ftok error.\n");
		exit(1);
	}

	if((msgid = msgget(msgkey, IPC_CREAT|0660)) == -1){
		printf("msgget error.\n");
		exit(1);
	}

	getmsgattr(msgid, msq_attr);

	msgbuffer.msgtype = 2;

	strcpy(msgbuffer.ctrlstring, "Another message.\n");

    if(msgsnd(msgid, &msgbuffer, sizeof(msgbuffer.ctrlstring), 0) == -1){
        printf("msgsnd error.\n");
        exit(1);
    }
    getmsgattr(msgid, msq_attr);
    msq_attr.msg_perm.uid = 3;
    msq_attr.msg_perm.gid = 2;
    
    if(msgctl (msgid, IPC_SET, &msq_attr) == -1){
        
        printf("msg set error.\n");
        exit(1);
    }

    getmsgattr(msgid, msq_attr);

    if(msgctl(msgid, IPC_RMID, NULL) == -1){
        
        printf("delete msg error.\n");
        exit(1);
    }
    
    getmsgattr(msgid, msq_attr);
    
    return 0;

}
void getmsgattr(int msgid, struct msqid_ds msq_info){
    
    if(msgctl(msgid, IPC_STAT, &msq_info) == -1){
    
        printf("msgctl error.\n");
        return;
    }
    printf("****information of message queue %d****", msgid);
    printf("last msgsnd to msg time is %d \n", ctime(&(msq_info.msg_stime)));
    printf("last msgrcv time from msg is %d\n", ctime(&(msq_info.msg_rtime)));
    printf("last chage msg time is %d \n", ctime(&(msq_info.msg_ctime)));
    printf("current number of bytes on queue is %ld\n", msq_info.msg_cbytes);
    printf("number os messages in queue is %ld \n", msq_info.msg_qnum);
    printf("max number of bytes on queue is %ld\n", msq_info.msg_qbytes);
    printf("pid of last msgsnd is %d\n", msq_info.msg_lspid);
    printf("pid of last msgrcv is %d\n", msq_info.msg_lrpid);

    printf("msg uid is %d\n", msq_info.msg_perm.uid);
    printf("msg gid is %d\n", msq_info.msg_perm.gid);
    printf("*****information end !*****\n");
}
