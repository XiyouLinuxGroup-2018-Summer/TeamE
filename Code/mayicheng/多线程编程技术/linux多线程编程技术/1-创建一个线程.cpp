#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "iostream"
//线程也是异步的，没有顺序的 
void test()
{
	pthread_exit(NULL);	//果真，但是这个是第3种方法，不是第二种
						//第二种是被其他线程取消 
}
using namespace std;
void *func(void *arg)//线程入口函数 
{
	printf("pthread start\n");
	test();
	printf("pthread end\n");
	return NULL;
}
void *func1(void *arg)//线程入口函数 
{
	while(1)
	{
		sleep(1);
		printf("pthread1 start\n");
	}
	return NULL;
}
//若控制线程结束，那么都结束了
//但是其他线程结束，不影响控制线程 
//任一线程调用了exit函数，整个进程就会终止。
//如果信号默认动作是终止进程，那么进程就终止了。

//线程结束方法
//1.return
//2.线程可以被被同一进程中的其他线程取消
//3.线程调用ptread_exit(NULL) 
int main()
{
	pthread_t thrd,thrd1;
	if(pthread_create(&thrd,NULL,func,NULL) != 0)
	{
 		cout << thrd <<  endl;	//没打出来这个是啥 
		printf("error is %s\n",strerror(errno));
	}
	if(pthread_create(&thrd1,NULL,func1,NULL) != 0)
	{
		printf("error is %s\n",strerror(errno));
	}
	while(1)
	sleep(1);
	return 0;
} 
