#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
//锁就最关键的时候用一下，其他时候就别用了！！！ 
//pthread_cancel
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//之和lock和unclock有关，只要++了其他都要队列排队
//不关注啥时候打开
//但是必须打开，不打开就出不来 
void *func(void *t)		 
{
	pthread_mutex_lock(&mutex);	//给mutex加锁，这是一条原子操作，不可能出现两个线程同时执行这个代码 
	int i = 7;					//异步的 
	while(i--)
	{
		sleep(1);
		printf("hello %d\n",*((int *)t));
	}
	pthread_mutex_unlock(&mutex);
}
void *func2(void *t)		 
{
	pthread_mutex_lock(&mutex);
	int i = 7;					 
	while(i--)
	{
		sleep(1);
		printf("hello %d\n",*((int *)t));
	}
	pthread_mutex_unlock(&mutex);
}
int main()
{
	pthread_t thr1,thr2;
	int a,b;
	a = 1;
	b = 2;
	if(pthread_create(&thr1,NULL,func,&a) != 0)
	{
		printf("error pthread_create\n");
	}
	if(pthread_create(&thr2,NULL,func2,&b) != 0)
	{
		printf("error pthread_create\n");
	}
	pthread_join(thr1,NULL);
	pthread_join(thr2,NULL);
	printf("main exit\n");
	return 0;
}
