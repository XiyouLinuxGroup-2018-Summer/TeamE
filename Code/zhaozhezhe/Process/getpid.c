#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){
	int pid;

	pid = fork();

	if(pid > 0){
		printf("I am Father process.PID is %d\n", getpid());
	}else if(pid == 0){
		printf("I am Child process. PID is %d\n", getpid());
	}else{
		printf("Fork error.\n");
	}
}
