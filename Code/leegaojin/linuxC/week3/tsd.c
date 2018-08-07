/*************************************************************************
	> File Name: tsd.c
	> Author:
	> Mail:
	> Created Time: 2018年08月06日 星期一 08时19分04秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
pthread_key_t key;

void* thread2(void *arg)
{
  long tsd = 5;
  printf("thread %d is running\n",pthread_self());
  //pthread_setspecific (key,(void*)tsd);
  //printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
  printf("thread %d returns %d\n",pthread_self(),tsd);
}
void * thread1(void*arg)
{
  long tsd ;
  pthread_t thid2;

  printf("thread %d is running\n",pthread_self());
  //pthread_setspecific(key,(void*)tsd);
  pthread_create(&thid2,NULL,thread2,NULL);
  sleep(2);
  //printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
  printf("thread %d returns %d\n",pthread_self(),tsd);
}
int main(void)
{
  pthread_t thid1;
  printf("main thread begins running\n");
  //pthread_key_create(&key,NULL);
  pthread_create(&thid1,NULL,thread1,NULL);
  sleep(3);
  //pthread_key_delete(key);
  printf("main thread exit\n");
  return 0;
}
