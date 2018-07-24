//same as cat

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("less ...\n");
		return 0;
	}
	char c[100];
	int fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		printf("%s\n",strerror((errno)));
	}
	else
	{
		while(read(fd,c,sizeof(c)) > 0)
		{
			printf("%s",c);
			memset(c,0,sizeof(c));
		}
	}
	close(fd);
	return 0;
}
