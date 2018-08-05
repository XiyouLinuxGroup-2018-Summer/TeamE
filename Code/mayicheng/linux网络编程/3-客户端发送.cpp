#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *func1(void *t)
{
	int st = *(int *)t;
	char s[100] = { 0 };
	while(1)
	{
		memset(s,0,sizeof(s));
		read(STDIN_FILENO,s,sizeof(s));
		if(send(st,s,strlen(s),0) == -1)
		{
			printf("3-client send error,%d\n",strerror(errno));
		}
	}
}
void *func2(void *t)
{
	int st = *(int *)t;
	char s[100] = { 0 };
	while(1)
	{
		memset(s,0,sizeof(s));
		if(recv(st,s,sizeof(s),0) == -1)
		{
			printf("3-client recv error,%d\n",strerror(errno));
		}
		printf("%s",s);
	}
}
int main(int argc,char *argv[])
{ 
	if(argc < 3)
	return -1;
	int port = atoi(argv[2]);
	int st = socket(AF_INET,SOCK_STREAM,0);	//AF_INET是用internet网络，SICK_STREAM是TCP协议 
	
	struct sockaddr_in addr;	//定义一个IP地址结构
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;	//设置结构地址类型为TCP/IP地址
	addr.sin_port = htons(port);	//指定一个端口号：8080，htons：将short类型从host字节类型转化为net字节类型 
	addr.sin_addr.s_addr = inet_addr(argv[1]);	//转化字符串，给addr结构
	
	if(connect(st,(struct sockaddr *)&addr,sizeof(addr)) == -1)
	{
		printf("connect faile,%s\n",strerror(errno));
		return 0;
	}
	while(1)
	{
		pthread_t thr1,thr2;
		if(pthread_create(&thr1,NULL,func1,&st) == -1)
		{
			printf("pthread_create error\n");
		}
		if(pthread_create(&thr2,NULL,func2,&st) == -1)
		{
			printf("pthread_create error\n");
		}
		pthread_join(thr1,NULL);
		pthread_join(thr2,NULL);
//		char s[100];
//		memset(s,0,sizeof(s));
//		read(STDIN_FILENO,s,sizeof(s));
//		if(send(st,s,strlen(s),0) == -1)
//		{
//			printf("send faile,%s\n",strerror(errno));
//			return 0;		
//		}
//		memset(s,0,sizeof(s));
//		if(recv(st,s,sizeof(s),0) == -1)
//		{
//			printf("recv faile,%s\n",strerror(errno));
//			return 0;
//		}
//		printf("%s",s);
	}
	return 0;
}
