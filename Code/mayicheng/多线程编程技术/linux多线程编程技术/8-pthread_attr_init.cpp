#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
//int pthread_equal(pthread_t th1,pthread_t th2);
//相同返回非0，不同返回0
//不要直接用等号判断，因为本来就不是普通类型 
void *func(void *t)
{
	while(1)
	{
		sleep(1);
		printf("hello 1\n");
	}
}
int main()
{
	pthread_t thrd1;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	
	if(pthread_create(&thrd1,&attr,func,NULL) != 0)
	{
		printf("error \n");
	 } 
	 pthread_attr_destroy(&attr);
	 pthread_join(thrd1,NULL);
	printf("no guanxi\n");
	sleep(1);
	return 0;
} 
