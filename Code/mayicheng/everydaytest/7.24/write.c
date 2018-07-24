#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char c[100] =  "a.txt";
	int fd = open(c,O_RDWR |O_APPEND);
	if(fd == -1)
	{
		printf("%s\n",strerror((errno)));
	}
	else
	{
		printf("fd = %d\n",fd);
		while(write(fd,c,strlen(c)) > 0)
		{
			memset(c,0,sizeof(c));
			scanf("%s",c);
		}
	}
	return 0;
}
