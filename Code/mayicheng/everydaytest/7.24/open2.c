#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
int main()
{
	close(0);
	char c[100] = "a.txt";
	int fd = open(c,O_WRONLY);
	if(fd == -1)
	{
		printf("%s\n",strerror(errno));
	}
	else
	{
		while(1)
		{
			scanf("%s",c);
			printf("%s",c);
		}
	}
	close(fd);
	return 0;
}
