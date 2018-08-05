//setsid()≤‚ ‘ 
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 

int main()
{
	int pid = fork();
	if(pid == 0)
	{
		setsid();
		sleep(100);
	}
	else
	{
		exit(0);
	}
	return 0;
} 
