#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//pthread_cancel
void *func(void *t)
{
	while(1)
	{
		sleep(1);
		printf("hello 1\n");
	}
}
void *func2(void *t)
{
	sleep(2);
	pthread_cancel(*((pthread_t*)t));
	int *p = (int *)malloc(sizeof(int));
	*p = 5;
	return (void *)p;
}
int main()
{
	pthread_t thr1,thr2;
	if(pthread_create(&thr1,NULL,func,NULL) != 0)
	{
		printf("error pthread_create\n");
	}
	if(pthread_create(&thr2,NULL,func2,&thr1) != 0)
	{
		printf("error pthread_create\n");
	}
	int *p1 = NULL;
	pthread_join(thr1,(void **)&p1);
	pthread_join(thr2,NULL);
	printf("main exit  %d\n",*p1);		//段错误，说明就没返回，p1是空的 
	return 0;
}
