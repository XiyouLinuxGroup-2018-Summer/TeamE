#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **environ) {

	pid_t pid;
	int stat_val;
	printf("Exec example.\n");
	pid = fork();

	switch(pid) {
		case -1:
		perror("Process create failed.\n");
		exit(1);
		case 0:
		printf("Child process is running\n");
		printf("My PID is %d, parent PID is %d\n", getpid(), getppid());
		printf("UID is %d, GID is %d\n", getuid(), getgid());
		execve("proccessimag", argv, environ);
		printf("This process never go here!\n");
		exit(0);
		defalut:
		printf("Parent process is running.\n");
		break;
	}

	wait(&stat_val);
	//return 0;
	exit(0);
}
