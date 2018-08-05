/*************************************************************************
	> File Name: jointhread.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月03日 星期五 21时06分12秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
void assisthread(void*arg)
{
    printf("I am helping to do some jobs\n");
    sleep(3);
    pthread_exit(0);
}
int main(void)
{
    pthread_t  assistthid;
    int         status;

    pthread_create(&assistthid,NULL,(void*)assisthread,NULL);
    pthread_join(assistthid,(void*)&status);
    printf("assisthread's exit is caused %d\n",status);

    return 0;
}
