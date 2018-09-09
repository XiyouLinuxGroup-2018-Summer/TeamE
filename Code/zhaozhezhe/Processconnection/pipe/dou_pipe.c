#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

void child_rw_pipe(int readfd, int writefd){
	char *message = "This is the message1, from child";
	write(writefd, message, strlen(message)+1);
	
	char message2[100];
	read(readfd, message2, 100);
	printf("Child read the message :%s \n", message2);
}
void parent_rw_pipe(int readfd, int writefd){
	char *message = "This is the message2, from parent";
	write(writefd, message, strlen(message)+1);
	
	char message2[100];
	read(readfd, message2, 100);
	printf("parent read the message :%s \n", message2);
}
int main(void){
	int pipe1[2], pipe2[2];
	pid_t pid;
	int stat_val;

	printf("realize full-duplex communication.\n");
	/*创建一个管道*/
	if(pipe(pipe1)){
		printf("Create pipe1 failed!\n");
		exit(1);
	}
	if(pipe(pipe2)){
		printf("Create pipe2 failed!\n");
		exit(1);
	}

	pid = fork();

	if(pid < 0){
		printf("Pid is error.\n");
		exit(1);
	}else if(pid == 0){
		close(pipe1[1]);
		close(pipe2[0]);
		child_rw_pipe(pipe1[0], pipe2[1]);
	}else if(pid > 0){
		close(pipe1[0]);
		close(pipe2[1]);
		parent_rw_pipe(pipe2[0], pipe1[1]);
		wait(&stat_val);
		exit(0);
	}

	return 0;
}
