/*************************************************************************
	> File Name: execve.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 15时59分20秒
 ************************************************************************/

#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char *argv[],char ** environ)
{
    pid_t pid;
    int stat_val;
    printf("Exec example!\n");
    pid = fork();
    switch(pid){
        case -1:
            perror("process Creation failed\n");
            exit(1);
        case 0 :
            printf("Child process is running\n");
            printf("My pid = %d,parentpid = %d\n",getpid(),getppid());
            printf("uid = %d,gid = %d\n",getuid(),getgid());
            execve("processimage",argv,environ);
            printf("process never go to here!\n");
        default:
            printf("Parent process is running\n");
            break;
    }

    wait(&stat_val);
    exit(0);
}

