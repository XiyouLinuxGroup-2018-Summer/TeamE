//3-利用共享内存通讯 
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
//一个写，一个读 
int main(int argc,char *argv[])
{
	char *buf;
	if(argc > 1)
	{
		buf = (char *)shmat(atoi(argv[1]),0,0);
		int pid = fork();
		if(pid == 0)
		{
			printf("%s\n",buf);	
		}
		if(pid > 0)
		{
			scanf("%s",buf);
		}
		shmdt(buf);
	}
	return 0;
} 
