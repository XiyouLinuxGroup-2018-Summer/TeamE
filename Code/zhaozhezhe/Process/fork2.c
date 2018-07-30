#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
	
	pid_t  _child;
	int k = 5;
	char *msg;
	_child = vfork();
	if(_child){
		msg = "Child process is running!";
		k = 3;
		/*
		while(k>0){
			//printf("%s\n", msg);
			puts(msg);
			sleep(1);
			k--;
		}
		*/
	}else{
		msg = "Father process is running!";
		k = 5;
		/*
		while(k>0){
			//printf("%s\n", msg);
			puts(msg);
			sleep(1);
			k--;
		}
		*/
	}

	while(k>0){
		//printf("%s\n", msg);
		puts(msg);
		sleep(1);
		k--;
	}

	exit(0);
	//return 0;
}
