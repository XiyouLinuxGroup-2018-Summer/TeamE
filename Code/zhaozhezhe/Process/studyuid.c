#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

extern int errno;

int main(void){
	
	int fd;
	printf("uid study :\n");
	printf("Process's UID is %d, EUID is %d.\n", getuid(), geteuid());

	if((fd = open("test.c", O_RDONLY)) == -1){
		printf("open failure, errno is %d : %s\n", errno, strerror(errno));
		exit(0);
	}else{
		printf("Open successfully!\n");
	}

	close(fd);

	return 0;
}
