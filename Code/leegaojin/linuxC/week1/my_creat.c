/*************************************************************************
	> File Name: my_creat.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 20时56分37秒
 ************************************************************************/
 
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
int main()
{
    int fd;

    if((fd=open("exmple_62.c",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))==-1){
//    if ((fd=creat("example_62.c",S_IRWXU))==-1){
       //     perror("open");
            printf("open:%s with errno:%d\n",strerror(errno),errno);
            exit(1);
        } 
        else{
            printf("creat file success\n");    
        }
    close(fd);
    return 0;
}
