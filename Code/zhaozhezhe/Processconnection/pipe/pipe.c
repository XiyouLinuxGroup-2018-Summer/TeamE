#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

void write_to_pipe(int fd){
	char message[100];
	read(fd, message, 100);
	printf("read from pipe :%s\n", message);
}

void read_to_pipe(int fd){
	char message[1000];
	peintf("Input your message!\n");
	scanf("%s", message);
	write(fd, message, strlen(message)+1);
}
int main(void){
	int fd[2];
	pid_t pid;
	int stat_val;

	/*创建一个管道*/
	if(pipe (fd)){
		printf("Create process failed!\n");
		exit(1);
	}

	pid = fork();

	if(pid < 0){
		printf("Pid is error.\n");
		exit(1);
	}else if(pid == 0){
		close(fd[1]);
		write_to_pipe(fd[0]);
	}else if(pid > 0){
		close(fd[0]);
		read_to_pipe(fd[1]);
		wait(&stat_val);
		exit(0);
	}

	return 0;
}
