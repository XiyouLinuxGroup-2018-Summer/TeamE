#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char **argv)
{
	int m = chmod(argv[1],0777);
	if(m == -1)
	{
		printf("~~%s\n",strerror(errno));	//strerror is in <string.h>
	}
	else
	{
		printf("ok\n");
	}
	return 0;
}
