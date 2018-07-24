#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd = creat("a.txt",O_WRONLY);
	if(fd == -1)
	{
		printf("%s",strerror(errno));
		return 0;
	}
	return 0;
}
