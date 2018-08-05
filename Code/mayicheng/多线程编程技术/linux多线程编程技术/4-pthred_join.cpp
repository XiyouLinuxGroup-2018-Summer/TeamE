#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "iostream"
using namespace std;
void *func(void *t)
{
	int m = 3; 				//这个是各3次，栈里边的 
	//static int m = 3;		//静态区的，一共3次 
	while(m--)
	{
 		sleep(1);
		printf("ptherad_create ok\n");
	}
	return NULL;
}
int main()
{
	pthread_t thrd,thrd1;		
	if(pthread_create(&thrd,NULL,func,NULL) != 0)	
	{
		printf("error\n");
	}
	if(pthread_create(&thrd1,NULL,func,NULL) != 0)	
	{
		printf("error\n");
	}
	pthread_join(thrd,NULL);		//死了就不等了
	//pthread_join还有其他功能 
	pthread_join(thrd1,NULL);		//主线程挂起，等待thrd1退出（死等）
									//类似wait 
	printf("main end!\n"); 			
	return 0;
} 
