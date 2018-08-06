#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
void run(void){
	printf("Function run is running in %lu.\n", pthread_self());
}
void *thread1(void *arg){
	pthread_t thid = pthread_self();
	printf("Current thread'1s ID is %lu.\n", thid);
	pthread_once( &once, run);
	printf("thread1 ends.\n");
}
void *thread2(void *arg){
	pthread_t thid = pthread_self();
	printf("Current thread'2s ID is %lu.\n", thid);
	pthread_once( &once, run);
	printf("thread2 ends.\n");
}
int main(void){
	pthread_t thid1, thid2;
	pthread_create(&thid1, 0, (void *)thread1, 0);
	pthread_create(&thid2, 0, (void *)thread2, 0);
	sleep(3);
	return 0;
}
