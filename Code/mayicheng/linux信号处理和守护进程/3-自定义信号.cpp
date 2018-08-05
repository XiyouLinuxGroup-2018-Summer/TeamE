//自定义信号 
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//SIGUSR1
//SIGUSR2
int statues = 0;
void mmm(int  stat)
{
	if(stat == SIGUSR1)
	{
		statues = 1;
	}
}
int main()
{
	signal(SIGUSR1,mmm);
	printf("%d\n",SIGUSR1);
	while(1)
	{
		if(statues)
		printf("hello world\n");
		sleep(1);
	}
} 
