/*************************************************************************
	> File Name: procread.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 08时39分52秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>

#define     FIFO_NAME   "myfifo"
#define     BUF_SIZE    1024

int main(void)
{
    int fd;
    char buf[BUF_SIZE];

    umask(0);
    fd = open(FIFO_NAME,O_RDONLY);
    read(fd,buf,BUF_SIZE);
    printf("Read content: %s\n",buf);
    close(fd);
    exit(0);
}
