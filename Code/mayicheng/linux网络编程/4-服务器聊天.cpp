#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
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
	if(argc < 2)
	return -1;
	int port = atoi(argv[1]);
	int st = socket(AF_INET,SOCK_STREAM,0);	//初始化socket 
	
	int on = 1;
	if(setsockopt(st,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1)
	{
		printf("setsocopt failed %s\n",strerror(errno));
		return 0;
	}
	
	struct sockaddr_in addr;	//定义一个IP地址结构
	memset(&addr,0,sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//INADDR_ANY代表server上所有地址
	//将IP和server程序绑定 
	if(bind(st,(struct sockaddr *)&addr,sizeof(addr)) == -1)
	{
		printf("bin failed %s\n",strerror(errno));
		return 0;
	}
	//server端开始listen。
	if(listen(st,20) == -1)
	{
		printf("listen failed %s\n",strerror(errno));
		return 0;
	}
	
	char s[1024];
	int client_st = 0;		//client端socket 
	socklen_t len = 0;
	struct sockaddr_in client_addr; 	//client端IP地址 
	void *p = &client_addr;
	int i;
	for(i = 0;i < 5;i++)
	{
		memset(&client_addr,0,sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		//accept会阻塞，直到有客户端连接过来，accept返回client的socket描述符 
		client_st = accept(st,(struct sockaddr *)&client_addr,&len);
		if(client_st == -1)
		{
			printf("accept error,%s\n",strerror(errno));
			return 0;
		}
		while(1)
		{
//		pthread_t thr1,thr2;		//别急，这有问题 
//		if(pthread_create(&thr1,NULL,func1,&st) == -1)
//		{
//			printf("pthread_create error\n");
//		}
//		if(pthread_create(&thr2,NULL,func2,&st) == -1)
//		{
//			printf("pthread_create error\n");
//		}
				pthread_t thr1,thr2;
		if(pthread_create(&thr1,NULL,func1,&client_st) == -1)
		{
			printf("pthread_create error\n");
		}
		if(pthread_create(&thr2,NULL,func2,&client_st) == -1)
		{
			printf("pthread_create error\n");
		}
		pthread_join(thr1,NULL);
		pthread_join(thr2,NULL);
//			memset(s,0,sizeof(s));
//			if(recv(client_st,s,sizeof(s),0) == -1)	//阻塞的 
//			{
//				printf("recv falied %s\n",strerror(errno));
//				close(client_st);
//				return 0;
//			}
//			printf("%s",s);
//			memset(s,0,sizeof(s));
//			read(STDIN_FILENO,s,sizeof(s));
//			if(send(client_st,s,strlen(s),0) == -1)
//			{
//				printf("send error!\n");
//				return 0;
//			}
//			//close(client_st);
		}
	}
	return 0;
} 
