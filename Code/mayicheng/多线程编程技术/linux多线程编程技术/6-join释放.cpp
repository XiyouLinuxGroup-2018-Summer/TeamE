#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int pthread_join(pthread_t th,void **thr_return);
//应该是返回值。。。 
//线程返回的是无类型的指针
void test()
{
	int  *a = (int *)malloc(sizeof(a));
	*a = 300;
	//pthreda_exit(void *)
	pthread_exit((void *)a);
 } 
void *func(void *t)
{
	test();			//如果不写，p是空，调用就错了 
	return NULL;
}
int main()
{
	pthread_t trd;
	int i = 100000;			//到67000挂了 
	//while(i--)
	if(pthread_create(&trd,NULL,func,NULL) != 0)		//&,==0是成功 
	{
		printf("error %d\n",100 - i);
	}
	int *p = NULL;
	pthread_join(trd,(void **)&p);				//和进程一样，释放掉线程。
	//（若不释放，死了之后，和进程一样，有一个结构体来维护着呢） 
	printf("*p = %d\n",*p);
	printf("success\n");		//这样就成功了 
	return 0;
} 
