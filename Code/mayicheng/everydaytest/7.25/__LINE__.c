//position

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("less ...\n");
		return 0;
	}
	int fd = open(argv[1],O_RDONLY);
	printf("%d\n",__LINE__);
	lseek(fd,0,SEEK_CUR);
	printf("%d\n",__LINE__);
	lseek(fd,0,SEEK_END);
	printf("%d\n",__LINE__);
	return 0;
}
