/*************************************************************************
	> File Name: processimage.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 15时55分11秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char *argv[],char ** eniron)
{
    int  i;

    printf("I am a process image!\n");
    printf("My pid = %d,parentpid = %d\n",getpid(),getppid());
    printf("uid = %d,gid = %d\n",getuid(),getgid());

    for(i=0;i<argc;i++)
        printf("argv[%d]:%s\n",i,argv[i]);
}
