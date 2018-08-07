#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg){
	int loops = *((int *)arg);
	int loc , j , s;

	for(j=0; j<loops; j++){
		s = pthread_mutex_lock(&mtx);
		if(s!=0){
			printf("lock error.\n");
		}
		loc = glob;
		loc++;
		glob = loc;
		s = pthread_mutex_unlock(&mtx);
		if(s!=0){
			printf("unlock error.\n");
		}
	}
}

int main(int argc, char **argv){
	pthread_t thid1, thid2;
	int loops, s;

	loops = 10000000;

	s = pthread_create(&thid1, NULL, threadFunc, &loops);
	if(s!=0){
		printf("create error.\n");
	}
	s = pthread_create(&thid2, NULL, threadFunc, &loops);
	if(s!=0){
		printf("create error.\n");
	}
	s = pthread_join(thid1, NULL);
	if(s!=0){
		printf("join error.\n");
	}
	s = pthread_join(thid2, NULL);
	if(s!=0){
		printf("join error.\n");
	}

	printf("glob = %d.\n", glob);
	
}
