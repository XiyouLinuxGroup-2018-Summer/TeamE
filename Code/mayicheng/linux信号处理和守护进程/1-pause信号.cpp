//只要有信号来，pause就返回 
#include <stdio.h>
#include <unistd.h> 
int main()
{
	pause();
	printf("ok....\n");
	return 0;
} 
