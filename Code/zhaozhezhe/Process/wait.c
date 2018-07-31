#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(void){

	pid_t pid;
	char *msg;
	int k;
	int exit_code;

	printf("Study how to get exit code\n");
	pid = fork();
	switch(pid){
	
		case 0:
			msg = "Child process is running .\n";
			k = 5;
			exit_code = 37;
			break;

		case -1:
			perror("Creat process is wrong.\n");
			exit(1);
		defalut:
			exit_code = 0;
			break;
	}

	if(pid != 0){
		int stat_val;
		pid_t child_pid;

		child_pid = wait(&stat_val);

		printf("Child procee has exited, PID is %d \n", child_pid);

		if(WIFEXITED(stat_val)){
			printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
		}else{
			printf("Child exit is abnormally.\n");
		}
	}else{
		while(k--){
			puts(msg);
			sleep(1);
		}
	}
	exit(exit_code);
	//return 0;
}
