#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;

char c[15][50] = { 0 };
//char *c[15];
char dirs[50] = { 0 };
char *arg[20] = { 0 };
void jiexi(char *p)		//解析命令
{
	int ppp = 0;
	while(arg[ppp])
	{
		free(arg[ppp]);
		arg[ppp++] = NULL;
	}
	int len = strlen(p);
	if(len > 100)
	return;
	memset(c,0,sizeof(c));		//清空之前的命令
	char *q1 = p,*q2 = p;		//好像叫长短指针
	while(*q1 == ' ')		//开始之前的清理工作
	{
		q1++;
	}
	q2 = q1;
	int m = 0;			//记录这个命令应该向哪放
	while(q1++)
	{
		if(q1 >= (p + len))
		{
			//c[m] = (char *)calloc(sizeof(q1 - q2 + 1),1);	
			strncpy(c[m++],q2,q1 - q2);
			return;
		}
		if(q1[0] == ' ')
		{
                        //c[m] = (char *)calloc(sizeof(q1 - q2 + 1),1);
			strncpy(c[m++],q2,q1 - q2);
     			while(q1 < p + len && (*q1) == ' ')                
      			{
                		q1++;
				if(q1 >= (p + len))
				return;
        		}
        		q2 = q1;
		}
	}
}
void cdcome()
{
	if(c[1][0] == 0)
	{
        	char sss[50] = { 0 };
       		getcwd(sss,sizeof(sss));
		chdir("/home/ma");
      	 	memset(dirs,0,sizeof(dirs));
        	strcpy(dirs,sss);
		return;
	}
	if(strcmp(c[1],"-") == 0)		//如果是 ‘-’
	{
		if(dirs[0] == 0)
		{
			return;
		}
		char c[50] = { 0 };
		getcwd(c,sizeof(c));
		chdir(dirs);
		printf("%s\n",dirs);
		memset(dirs,0,sizeof(dirs));
		strcpy(dirs,c);
		return;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@@@@@@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        char sss[50] = { 0 };
        getcwd(sss,sizeof(sss));
	if(chdir((const char*)c[1]) < 0)
	{
		printf("%s\n",(const char*)c[1]);
		printf("set dir error,%s\n",strerror(errno));
		return;
	}
        memset(dirs,0,sizeof(dirs));
        strcpy(dirs,sss);
}

void founds()
{
	char *mm[] = { "/usr/bin","bin","./"};
	if(strncmp(c[0],"./",2) == 0)
	return;
	int i = 0;
	for(;i < 3;i++)
	{
		DIR *dp;
		dp = opendir(mm[i]);
		struct dirent *p;
		while((p = readdir(dp)) != NULL)
		{
			if(strcmp(c[0],p->d_name) == 0)
			{
				char abc[100] = { 0 };
				strcpy(abc,mm[i]);
				if(abc[strlen(abc) - 1] != '/')
				abc[strlen(abc)] = '/';
				strcat(abc,c[0]);
				strcpy(c[0],abc);
				return;
			}
		}
	}
	printf("can't find this commid\n");
	exit(0);
} 
void founds(int t)
{
        char *mm[] = { "/usr/bin","bin","./"};
        if(strncmp(c[t],"./",2) == 0)
        return;
        int i = 0;
        for(;i < 3;i++)
        {
                DIR *dp;
                dp = opendir(mm[i]);
                struct dirent *p;
                while((p = readdir(dp)) != NULL)
                {
                        if(strcmp(c[t],p->d_name) == 0)
                        {
                                char abc[100] = { 0 };
                                strcpy(abc,mm[i]);
                                if(abc[strlen(abc) - 1] != '/')
                                abc[strlen(abc)] = '/';
                                strcat(abc,c[t]);
                                strcpy(c[t],abc);
                                return;
                        }
                }
        }
        printf("can't find this commid\n");
	exit(0);
}
void changess(int j)
{
	int i = 0;
      	for(;c[j][0] != 0;i++,j++)
      	{
                if((strcmp(c[j],"&")) == 0)
                {
			return;
                }
                if((strcmp(c[j],"|")) == 0)
                {
			return;
                }
                if((strcmp(c[j],">")) == 0)
                {
			return;
                }
                if((strcmp(c[j],">>")) == 0)
                {
			return;
                }
                if((strcmp(c[j],"<")) == 0)
                {
			return;
                }
		arg[i] = (char *)calloc(sizeof(strlen(c[j]) + 1),1);
		strcpy((char *)arg[i],c[j]);
        }
}
void zhixing()
{
	int yv = 0;
	int greps = 0;
	int srcdx = 0;
	int sccdx = 0;
	int sccdx2 = 0;
	int i = 0;
	int j = 0;		//j来判断位置，为以后方便
	int k = 0;
        for(;c[i][0] != 0;i++)
        {
                if((strcmp(c[i],"&")) == 0)
                {
                        if(c[i + 1][0] != 0)
                        {
                                printf("格式不对！\n");
                                return;
                        }
			k = i;
                        yv++;
                }
                if((strcmp(c[i],"|")) == 0)
		{
			if(c[i + 1][0] == 0)
			{
				printf("格式不对！\n");
				return;
			}
			greps++;
			j = i;
		}
                if((strcmp(c[i],">")) == 0)
                {
                        if(c[i + 1][0] == 0)
                        {
                                printf("格式不对！\n");
                                return;
                        }
                        sccdx++;
			j = i;
                }
                if((strcmp(c[i],">>")) == 0)
                {
                        if(c[i + 1][0] == 0)
                        {
                                printf("格式不对！\n");
                                return;
                        }
                        sccdx2++;
			j = i;
                }
                if((strcmp(c[i],"<")) == 0)
                {
                        if(c[i + 1][0] == 0)
                        {
                                printf("格式不对！\n");
                                return;
                        }
                        srcdx++;
			j = i;
                }
        }
	if((srcdx + sccdx2 + sccdx + greps) > 1)
	{
            	printf("格式不对！\n");
                return;
        }
	if(yv > 1)
	{
                printf("格式不对！\n");
                return;
	}
                

	//printf("j = %d\n",j);
        if(strcmp(c[0],"cd") == 0)
        {
        	cdcome();
        	return;
      	}
//
	int pid = fork();
	if(j == 0 && pid == 0)		//只有一个命令的时候
	{
		founds();
		changess(0);
		execve(c[0],arg,0);
		exit(0);
	}
        if(pid == 0 && sccdx == 1)		//有>的时候
        {
                founds();
                changess(0);
		int fd = open(c[j + 1],O_RDWR | O_CREAT | O_TRUNC,0644);
		dup2(fd,1);
                execve(c[0],arg,0);
                exit(0);
        }
        if(pid == 0 && sccdx2 == 1)              //有>>的时候
        {
                founds();
                changess(0);
                int fd = open(c[j + 1],O_CREAT | O_APPEND | O_RDWR,0644);
                dup2(fd,1);
                execve(c[0],arg,0);
                exit(0);
        }
        if(pid == 0 && srcdx == 1)              //有<的时候
        {
                founds();
                changess(0);
                int fd = open(c[j + 1],O_RDONLY);
                dup2(fd,0);
                execve(c[0],arg,0);
                exit(0);
        }
        if(pid == 0 && greps == 1)              //有|的时候
        {
		int fd = open("/home/ma/fssh/fifo1",O_RDWR | O_CREAT | O_TRUNC,0644);
		int pid = fork();
		if(pid == 0)
		{
			//sleep(1);
                	founds(j + 1);
                	//printf("zhixing %s\n",c[j + 1]);
                	changess(j + 1);
                	dup2(fd,0);
                	execve(c[j + 1],arg,0);
                	exit(0);
		}
		if(pid > 0)
		{
			founds();
             		//printf("zhixing %s\n",c[0]);
                	changess(0);
                	dup2(fd,1);
                	execve(c[0],arg,0);
			int sss;
			wait(&sss);
                	exit(0);
		}
        }



	if(pid > 0)
	{
		if(yv == 0)			//如果有&，就结束，没有就等等
		{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			int stat_val;
			wait(&stat_val);
			return;
		}
		else
		{
			return;
		}
	}

}
void mmm(int a)
{
	if(a == SIGINT)
	{
		printf("该信号已失效！\n");
		return;
	}
}
int main()
{
	signal(SIGINT,mmm);
	int flags = 1;
	while(1)
	{	
		if(flags)
		{
			getcwd(dirs,sizeof(dirs));
			flags = 0;
		}
		char *p = readline("\033[31m[myshell:\033[0m");
		add_history(p);
		jiexi(p);
		zhixing();	
	}
	return 0;
}
