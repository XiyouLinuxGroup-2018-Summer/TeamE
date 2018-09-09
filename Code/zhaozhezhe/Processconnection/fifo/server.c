#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define RD_FIFO "readfifo"
#define WR_FIFO "writefifo"
#define buf_size 1024

int main(void){
	int wfd,rfd;
	char buf[buf_size];
	int len;

	umask(0);
	if(mkfifo(WR_FIFO, S_IFIFO | 0666) == -1){
		printf("can't create fifo %s because %s.\n", WR_FIFO, strerror(errno));
		exit(1);
	}
	umask(0);
	wfd = open(WR_FIFO, O_WRONLY);
	if(wfd == -1){	
		printf("open fifo %s error because %s.\n", WR_FIFO, strerror(errno));
		exit(1);
	}
	while ((rfd = open(RD_FIFO, O_RDONLY)) == -1){sleep(1);}
	while (1){
		printf("Server: ");
		fgets(buf, buf_size, stdin);
		buf[strlen(buf)-1] = '\0';
		if(strncmp (buf, "quit", 4) == 0){
			close(wfd);
			unlink(WR_FIFO);
			close(rfd);
			exit(0);
		}
		write(wfd, buf, strlen(buf));

		len = read(rfd, buf, buf_size);
		if(len > 0){
			buf[len] = '\0';
			printf("Client: %s\n", buf);
		}
	}
}
