//patrent


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
	int fd = open(c,O_RDONLY);
	if(fd == -1)
	{
		printf("%s\n",strerror((errno)));
	}
	else
	{
		while(read(fd,c,sizeof(c)) > 0)
		{
			printf("%s",c);
		}
	}
	return 0;
}
