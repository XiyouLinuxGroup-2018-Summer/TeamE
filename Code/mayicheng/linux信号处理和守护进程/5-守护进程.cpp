//创建守护进程 
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <syslog.h> 
//syslog提供服务
//openlog函数打开日志，syslog写入日志，closelog关闭日志
//也可以不调用openlog和closelog 
int main()
{
	int pid = fork();
	if(pid > 0)
	{
		exit(0);
	}
	if(pid == 0)
	{
		setsid();
		chdir("/");
		umask(0);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	syslog(LOG_INFO,"mydaemin is OK\n");
	while(1)
	{
		sleep(1);
	}
	return 0;
}
