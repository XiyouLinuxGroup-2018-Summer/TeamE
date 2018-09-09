/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 17时58分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define buf_size 1024
#define pid 32
#define pathname "/tmp"
#define server_msg 1
#define client_msg 2

int main(void){
    struct my_msgbuffer{
        long msgtype;
        char ctrlstring[buf_size];
    }msgbuffer;

    int gid;
    int msglen;
    key_t msgkey;

    if((msgkey = ftok(pathname, pid)) == -1 ){
        printf("ftok error.\n");
        exit(1);
    }
    if((gid = msgget(msgkey, IPC_CREAT | 0660)) == -1){
        printf("msgget error.\n");
        exit(1);
    }

    while(1){
        printf("Server : ");
        
        fgets(msgbuffer.ctrlstring, buf_size, stdin);

        if(strncmp("exit", msgbuffer.ctrlstring, 4) == 0){
            msgctl(gid, IPC_RMID, NULL);
            break;
        }
        msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring)-1] = '\0';
        msgbuffer.msgtype = server_msg;

        if(msgsnd(gid, &msgbuffer, strlen(msgbuffer.ctrlstring)+1, 0) == -1){
            printf("msgsnd error.\n");
            exit(1);
        }

        if(msgrcv(gid, &msgbuffer, buf_size, client_msg, 0) == -1){
            printf("msgrcv error.\n");
            exit(1);
        }

        printf("Client :%s\n", msgbuffer.ctrlstring);
    }
}
