#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define fifo_name "fifo"
#define buf_size 1024

int main(void){
	int fd;
	char buf[buf_size] = "hello, wfifo!.";

	umask(0);

	if(mkfifo(fifo_name, S_IFIFO | 0666) == -1){
		printf("mkfifo error.\n");
		exit(1);
	}

	if(fd = open(fifo_name, O_WRONLY) == -1){
		printf("open error.\n");
		exit(1);
	}
	write(fd, buf, strlen(buf)+1);

	close(fd);
	exit(0);
}
