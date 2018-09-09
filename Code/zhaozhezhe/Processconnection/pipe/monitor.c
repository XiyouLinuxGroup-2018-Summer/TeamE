#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	int fd[2];
	pid_t pid;
	int stat_val;

	if(argc < 2){
		printf("wrong argv");
		exit(0);
	}
	if(pipe(fd)){
		printf("pipe failed.\n");
		exit(1);
	}

	pid = fork();

	if(pid < 0){
		printf("pid is failed.\n");
		exit(1);
	}else if(pid == 0){
		close(0);
		dup(fd[0]);
		execve("ctr", (void *)argv, __environ);
		exit(0);
	}else if(pid > 0){
		close(fd[0]);
		write(fd[1], argv[1], strlen(argv[1]));
	}

	wait(&stat_val);
	exit(0);
	//return 0;
}
