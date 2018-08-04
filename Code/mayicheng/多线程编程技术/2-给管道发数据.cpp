//2-给管道发数据 
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
//也可以直接echo ‘’和cat，但是执行完就会退出了（关闭了） 
//程序一旦退出，操作系统会自动清理，所以都没了 
void writefile()
{
	int fd = open("/home/ma/fssh/fifo1",O_WRONLY);
	if(fd < 0)
	printf("open file error,%s\n",strerror(errno));
	char buf[100] = { 0 };
	scanf("%s",buf);
	write(fd,buf,sizeof(buf));
	close(fd);	//关闭了这个管道 
	return;
}
int main()
{
	writefile();
	return 0;
}
