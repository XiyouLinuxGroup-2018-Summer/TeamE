/*************************************************************************
	> File Name: procwrite.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 08时44分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define FIFO_NAME   "myfifo"
#define     BUF_SIZE    1024

int main(void)
{
    int fd;
    char buf[BUF_SIZE] = "Hello procwrite,I come process named procread!";

    umask(0);

    if(mkfifo(FIFO_NAME,S_IFIFO|0666)==-1)
    {
        perror("mkfifo error!");
        exit(1);
    }
    if((fd = open(FIFO_NAME,O_WRONLY)) == -1)
    {
        perror("fopen error!");
        exit(1);
    }
    write(fd,buf,strlen(buf)+1); //向FIFO写数据

    close(fd);
    exit(0);
}
