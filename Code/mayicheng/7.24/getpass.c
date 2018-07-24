#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	char *q = "ehifugig";
	printf("%d\n",q);
	q = malloc(100);
	printf("%d\n",q);
	char *p = getpass("plese tip:");
	p[1] = '2';
	printf("%s\n",p);
	printf("%d\n",p);
	int a;
	printf("%d\n",&a);
	return 0;
}
