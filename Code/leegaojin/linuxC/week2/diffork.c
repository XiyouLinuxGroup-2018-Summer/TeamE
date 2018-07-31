/*************************************************************************
	> File Name: diffork.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 11时33分53秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int globVar = 5;

int main(void)
{
    pid_t pid ;
    int     var = 1,i;
    printf("fork is diffirent with vfrok \n");
    
    //pid = fork();
    pid = vfork();
    switch(pid) {
        case 0:
            i=3;
            while(i-->0)
            {
                printf("Child process os running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Child's globVar = %d,var = %d\n",globVar,var);
            exit(0);     //有坑见系统编程P188
        case -1:
            perror("process creation failed\n");
            exit(0);
        default:
            i=5;
            while(i-->0)
            {
                printf("Parent process is running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Parent's globVar = %d,var = %d\n",globVar,var);
            exit(0);
    }
}
