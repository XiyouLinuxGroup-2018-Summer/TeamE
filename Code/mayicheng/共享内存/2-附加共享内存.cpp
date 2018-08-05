//2-附加共享内存区 (连起来和释放)
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
//void *shmat(int shmid,const void *shmaddr,int shmflg);
//int shmdt(const void *shmaddr);
int main(int argc,char *argv[])
{
	char *buf = NULL;
	int shmid = 0;
	if(argc > 1)	//这个填你共享内存那个 
	{
		shmid  = atoi(argv[1]);
		buf = (char *)shmat(shmid,0,0);
		sleep(60);
		shmdt(buf);
	}
	return 0;
} 
