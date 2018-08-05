//守护和管道通信-1
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

void readfile()
{
	int fd = open("/home/ma/fssh/fifo1",O_RDONLY);
	if(fd < 0)
	printf("open file error,%s\n",strerror(errno));
	char buf[100] = { 0 };
	int len;
	while((len = read(fd,buf,sizeof(buf))) > 0)
	{
		printf("%s",buf);
		memset(buf,0,sizeof(buf));
	}
	close(fd);
	return;
}
void daem()
{
	int pid = fork();	//1.fork出一个子进程 
	if(pid > 0)
	exit(0);			//2.父进程退出 
	if(pid == 0)
	{
		setsid();		//3.子进程脱离控制 
		chdir("/");		//4.改变工作目录
						//5.（可以不弄这个）umask，关闭不必要的文件描述符 
	}
	return;
}
int main()
{
	daem();
	readfile();
	return 0;
}
 
