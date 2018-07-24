#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("less ...\n");
		return 0;
	}
	int fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		printf("%s\n",strerror(errno));
		return 0;
	}
	else
	{
		printf("succeed fd = %d\n",fd);
		struct stat buf;
		fstat(fd,&buf);
		if(S_ISREG(buf.st_mode))
		{
			printf("%s is charfile\n",argv[1]);
		}	
	}
	close(fd);
	return 0;
}
