#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "iostream"
using namespace std;
//为什么能同时调用这个函数？？
//因为：代码段是代码段，系统看见创建线程，就会给它分配栈，然后个是个的，不影响	
//同时也是异步的 
void *func(void *t)			
{
	cout << t << endl; 
	int i = *(int *)t;
	while(1)
	{
		sleep(1); 
		printf("ptherad_create ok  %d\n",i);
	}
	return NULL;
}
void threadcreat()		//果真！！千万记得这个i1i2(栈的，这个程序结束就没了)，一旦有个问题，就段错误了 
{						//也可以弄到全局或者堆区 
	int i1 = 1,i2 = 2;
	pthread_t thrd1,thrd2;			//先弄个这个 
	cout << "&i1 = " << &i1 << " &i2 = " << &i2 << endl;
	if(pthread_create(&thrd1,NULL,func,&i1) != 0)	//开启多线程模式 
	{
		printf("error\n");
	}
	if(pthread_create(&thrd2,NULL,func,&i2) != 0)	//开启多线程模式 
	{
		printf("error\n");
	}
	while(1)
	sleep(1);
}
int main()
{
	int i1 = 1,i2 = 2;
	pthread_t thrd1,thrd2;			//先弄个这个 
	if(pthread_create(&thrd1,NULL,func,&i1) != 0)	//开启多线程模式 
	{
		printf("error\n");
	}
	if(pthread_create(&thrd2,NULL,func,&i2) != 0)	//开启多线程模式 
	{
		printf("error\n");
	}
	//threadcreat();
	while(1)
	sleep(1); 		
	return 0;
} 
