#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	char c[100];
	int i = 0;
	memset(c,1,sizeof(c));
	memset(c,65,sizeof(c));
//	for(i = 0;i < 100;i++)
//	{
//		printf("%c",c[i]);
//	}
//	printf("\n");
	read(STDIN_FILENO,c,sizeof(c));
	write(STDOUT_FILENO,c,sizeof(c));
//	printf("%d",c[5]);
//	for(i = 0;i < 100;i++)
//	{
//		printf("%d ",c[i]);
//	}
	return 0;
}
