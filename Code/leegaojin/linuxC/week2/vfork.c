/*************************************************************************
	> File Name: vfork.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 14时40分23秒
 ************************************************************************/
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>

int      globvar = 6;

int main(void)
{
    int     var;
    pid_t   pid;

    var = 88;
    printf("before vfork\n");
    if((pid = vfork())<0){
//        err_sys("vfork error");
                ;
    }else if(pid == 0){
        globvar++;
        var++;
        _exit(0);
    }
    //parent continues here
    printf("pid = %ld,glob = %d,var=%d\n",(long)getpid,globvar,var);
    exit(0);

}

