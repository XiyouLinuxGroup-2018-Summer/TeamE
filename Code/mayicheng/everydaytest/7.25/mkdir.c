//have question
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
	int tt = umask(0);
	printf("umask = %o\n",tt);
	if(argc < 2)
	{
		printf("less ...\n");
		return 0;
	}
	//int t = mkdir(argv[1],0777|S_IWOTH);
	int t = mkdir(argv[1],0555);
	if(t == -1)
	{
		printf("%s\n",strerror(errno));
	}
	else
	{
		printf("ok\n");
	}
	tt = umask(0);
	printf("tt = %o\n",tt);
	return 0;
}
