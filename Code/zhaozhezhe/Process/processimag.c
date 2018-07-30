#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char **argv, char **environ) {

	int i;

	printf("I am a process imag.\n");
	printf("My PID is %d, My parent PID is %d.\n", getpid(), getppid());
	printf("UID = %d GID = %d.\n", getuid(), getgid());

	for (i=0; i<argc; i++) {
		printf("argv[%d] = %s \n", i, argv[i]);
	}

	return 0;
}
