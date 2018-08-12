#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* threadA(void*arg)
{
  printf("A");
  pthread_exit(0);
}
void* threadB(void*arg)
{
  printf("B");
  pthread_exit(0);
}
void* threadC(void*arg)
{
  printf("C");
  pthread_exit(0);
}
int main()
{
  pthread_t thid1,thid2,thid3;
  int status;

  while(1)
  {
    pthread_create(&thid1,NULL,threadA,NULL);
    pthread_join(thid1,(void*)&status);
    pthread_create(&thid2,NULL,threadB,NULL);
    pthread_join(thid2,(void*)&status);
    pthread_create(&thid3,NULL,threadC,NULL);
    pthread_join(thid3,(void*)&status);
  }
  return 0;
}
