#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#define MAX_THREAD 3 //线程个数

unsigned long long main_counter, counter[MAX_THREAD];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void* arg);

int main()
{
  int i, rtn, ch;
  int tmp[MAX_THREAD];
  pthread_t pthread_id[MAX_THREAD] = { 0 }; // 存放线程id
  for (i = 0; i < MAX_THREAD; i++)
  {
    if(pthread_create(&pthread_id[i],NULL,(void*)thread_worker,&i)!=0){
       printf("thread creation failed\n");
      }
/* 在这里填写代码,创建一个普通的线程, 线程id存入pthread_id[i],
线程执行函数为thread_worker(), 并将i作为参数传给线程*/
  }
    do //用户按一次回车执行下面的循环体一次,按q退出
    {
    unsigned long long sum = 0;
//求所有线程的 counter 的和
    for (i = 0; i < MAX_THREAD; i++)
    {
        sum += counter[i];
        printf("%llu ", counter[i]);
    }
    printf("%llu/%llu", main_counter, sum);
    } while ((ch = getchar()) != 'q');
  return 0;
}
void* thread_worker(void* arg)
{
  int thread_num;
  thread_num=*(int*)arg;
// 在这里填写代码, 把 main 中的i值传递给thread_num
  for (; ;)
  {
    counter[thread_num]++;
    main_counter++;
  }
}
