#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
//pthread_detach(pthread_t th);			//不能自己给自己detach了 
//一旦线程成为了可分离线程，就和控制线程（或者其他线程）没关系了，不能使用pthread_join了 

//可分离线程的使用场景（死了不记录struct信息） 
//1.主线程不需要等待子线程
//2.主线程不关系子线程的返回码 
void *func(void *t)
{
	printf("hello\n");
	return NULL;
}
int main()
{
	pthread_t thr;
	if(pthread_create(&thr,NULL,func,NULL) != 0)
	{
		printf("error pthread_create\n");
	}
	pthread_detach(thr);
	pthread_join(thr,NULL);
	printf("no guanxi\n");
	sleep(2);
	return 0;
} 
