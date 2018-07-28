/*************************************************************************
	> File Name: fcntl_access.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 14时51分17秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
void my_err(const char*arr_string,int line)
{
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(1);
}

int main()
{
    int         ret;
    int         access_mode;
    int         fd;
    if((fd=open("example_64",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1){
        my_err("open",__LINE__);
        exit(1);
    }
}
