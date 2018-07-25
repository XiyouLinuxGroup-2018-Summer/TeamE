//if use ftruncate,you should set open mode that can write
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("less ...\n");
		return 0;
	}
	//int fd = open(argv[1],O_RDONLY);
	int fd = open(argv[1],O_WRONLY);
	if(fd == -1)
	{
		printf("open -> %s\n",strerror(errno));
		return 0;
	}
	int m = ftruncate(fd,100);
	if(m == -1)
	{
		printf("ftruncate -> %s\n",strerror(errno));
		return 0;
	}
	printf("ftruncate ok~\n");
	return 0;
}
