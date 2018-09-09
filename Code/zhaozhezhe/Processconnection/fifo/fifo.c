#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define fifo_name "fifo"
#define buf_size 1024

int main(void){
	int fd;
	char buf[buf_size];
	
	umask(0);
	
	fd = open(fifo_name, O_RDONLY);
	read(fd, buf, buf_size);
	printf("read contect: %s\n", buf);
#if 0
	if(mkfifo(fifo_name, S_IFIFO | 0666) == -1){
		printf("mkfifo error");
		exit(1);
	}

	if(fd = open(fifo_name, O_WRONLY) == -1){
		printf("open error.\n");
		exit(1);
	}

	write (fd, buf, strlen(buf)+1);
#endif
	close(fd);

	exit(0);
}
