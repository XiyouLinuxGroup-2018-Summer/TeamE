//signal
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
// *** //说说个人理解：
//首先把方法放在最前面，就是说信号来了该怎么办
//一旦信号来了，cpu那电压变高之类的，暂停所有工作，然后调用上面的方法，
//把信号传进来，然后执行这个函数

//SIGSTOP SIGKILL 不能捕捉，直接退出 
void mmm(int sign)
{
	switch(sign)
	{
		case SIGINT:
			printf("SIGINT Signal\n");
			exit(0);
		case SIGALRM:
			printf("SIGALRM Signal\n");
			//alarm(5);		//这样就定时了  
			//STGALRM 默认是程序退出 
			break;
		case SIGKILL:	//实践证明，确实是，不能被捕获，只能死 
			printf("SIGKILL signal\n");
			exit(0); 
	}
}
//raise	给自己发信号
//alarm 也是给自己发信号	(就1次，除非while)
//int alarm(unsigned int seconds)
//about SIGBRT 即使捕捉到，程序也会异常退出
//自定义sleep函数 
//wait函数也是信号，pause，SIGCHID（什么也不干）-子进程已经停止或终止 
int main()
{
	signal(SIGINT,mmm);//其实SIGINT和SIGALRM都是整数，没啥 
	signal(SIGALRM,mmm);//SIGINT是2，SIGALRM是14
	//就是给操作系统说了，登记了
	signal(SIGKILL,mmm); 
	printf("SIGKILL = %d\n",SIGKILL);
	while(1)
	{
		alarm(5);
		pause(); 	//一直在等待 
		//sleep(1);
		printf("hello...\n");
	}
	return 0;
}
