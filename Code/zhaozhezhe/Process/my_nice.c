#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>

int main(void){

	pid_t pid;
	int stat_val;
	int oldpri, newpri;

	printf("Nice Study\n");

	pid = fork();

	if(pid == 0){
			printf("Child is running, CurPid is %d, ParentPid is %d\n", pid, getpid());
			oldpri = getpriority(PRIO_PROCESS, 0);
			printf("Old priority is %d\n", oldpri);

			newpri = nice(2);
			printf("New priority = %d \n", newpri);

			exit(0);
	}if(pid == -1){
			perror("Process Creation failed\n");
	}
	if(pid > 0){
			printf("Parent is running, Children is %d, ParentPId is %d\n", pid, getpid());
	}
	wait(&stat_val);
	exit(0);
}
