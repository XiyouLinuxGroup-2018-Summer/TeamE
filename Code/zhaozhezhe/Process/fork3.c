#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(void){

	pid_t pid;
	pid = fork();
	/*
	if(pid>0){
		printf("I am parent process, my pid is %d\n", getpid());
		exit(0);
	}
	*/
	/*
	if(pid == 0){
		while(1){
			printf("A background process, PID %d, Parent PID %d", getpid(), getppid());
			sleep(1);
		}
	}
	if(pid>0){
		printf("I am parent process, my pid is %d\n", getpid());
		exit(0);
	}
	*/
	switch(pid){
		case 0:
		while(1){
			printf("A background process, PID %d, Parent PID %d\n", getpid(), getppid());
			sleep(3);
		}
		case -1:
		perror("process creation failed!\n");
		exit(-1);
		defalut:
		printf("I am parent process, my pid is %d\n", getpid());

	}
	return 0;
}
