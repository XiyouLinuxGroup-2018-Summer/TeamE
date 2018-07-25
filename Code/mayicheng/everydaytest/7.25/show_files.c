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
	DIR *p = opendir(argv[1]);
	struct dirent *ptr;
	while((ptr = readdir(p)) != NULL)
	{
		printf("%s\n",ptr->d_name);
	}
	return 0;
}
