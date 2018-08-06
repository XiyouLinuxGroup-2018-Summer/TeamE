#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *thread(void){
	pthread_t newthid;
	newthid = pthread_self();
	printf("This is the new pthread Id %lu.\n", newthid);
}
int main(void){
	pthread_t thid;
	printf("main ptreah.Id is %lu.\n", pthread_self());
	if (pthread_create(&thid, NULL, (void *)thread, NULL)){
		printf("thread error.\n");
	}
	sleep(1);
	return 0;
}
