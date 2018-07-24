#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
//	close(STDOUT_FILENO);
	char s[] = "a.txt";
	int fd = open(s,O_RDONLY);
	//int fd = open(s,O_RDONLY,S_IRUSR);
	//int fd = open(s,O_WRONLY);
	printf("fd = %d\n",fd);
	if(fd == -1)
	{
		printf("%s\n",strerror(errno));
	}
	else
	close(fd);
	return 0;
}
