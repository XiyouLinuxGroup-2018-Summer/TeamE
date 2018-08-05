//3-守护和管道通信小技巧 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

void lread()
{
	int fd = open("/home/ma/fssh/fifo1",O_RDONLY);
	if(fd < 0)
	printf("open file error,%s\n",strerror(errno));
	char buf[100] = { 0 };
	read(fd,buf,sizeof(buf));
	if(buf[strlen(buf) - 1] == '\n')
	buf[strlen(buf) - 1] = 0;
	close(STDOUT_FILENO);	//关闭标准输出 
	open(buf,O_WRONLY); 	//打开这个tty的 
	close(fd);
	return;
}
void daem()
{
	int pid = fork();
	if(pid > 0)
	exit(0);
	if(pid == 0)
	{
		setsid();
		chdir("/");
	}
	return;
}
void mmm(int a)
{
	if(a == SIGINT)
	lread();
}
int main()
{
	signal(SIGINT,mmm);
	daem();
	while(1)
	{
		printf("hello world!\n");
		sleep(1);
	}
	return 0;
}
 
