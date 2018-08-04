#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "iostream"
//编译的时候要加 -lpthread 
//ldd查看
//nm全看看 
using namespace std;
void *func(void *t)			//进入，退出 
{
	printf("ptherad_create ok\n");
	return NULL;
}
int main()
{
	pthread_t thrd;			//先弄个这个 
	if(pthread_create(&thrd,NULL,func,NULL) != 0)	//开启多线程模式 
	{
		printf("error\n");
	}
	sleep(1); 			//真的是异步的，控制线程结束了什么都没了
						//其它线程结束不影响控制线程 
						//控制线程睡一秒，上面我那个就执行完成了 
	return 0;
} 
