#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
void *func(void *t)
{
	return NULL;
}
int main()
{
	pthread_t trd;
	int i = 100000;			//到67000挂了 
	while(i--)
	if(pthread_create(&trd,NULL,func,NULL) != 0)		//&,==0是成功 
	{
		printf("error %d\n",100 - i);
		break;
	}
	return 0;
} 
