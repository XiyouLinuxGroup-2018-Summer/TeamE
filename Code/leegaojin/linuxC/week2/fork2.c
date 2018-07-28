/*************************************************************************
	> File Name: fork2.c
	> Author:
	> Mail:
	> Created Time: 2018年07月28日 星期六 11时34分09秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(void)
{
    pid_t pid;
    char * mag;
    int k;

    printf("Proccess Creation Study\n");
    pid = fork();
    switch(pid){
        case 0:
            mag = "Child proccess is running";
            k = 3;
            break;
        case -1:
            perror("Proccess creation failed\n");
            break;
        default:
            mag ="Parent proccess is running";
            k =5;
            break;
    }
    while(k>0)
    {
        puts(mag);
        printf("**********\n");
        sleep(1);
        k--;
    }
    exit(0);
}
