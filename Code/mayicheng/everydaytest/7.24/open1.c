#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
int main()
{
	close(1);
	char c[100] = "/dev/tty3";
	int fd = open(c,O_WRONLY);
	if(fd == -1)
	{
		printf("%s\n",strerror(errno));
	}
	else
	{
		while(1)
		{
			printf("fd = %d\n",fd);
			sleep(1);
		}
	}
	close(fd);
	return 0;
}
