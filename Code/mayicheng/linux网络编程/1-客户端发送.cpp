#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{ 
	int st = socket(AF_INET,SOCK_STREAM,0);	//AF_INET是用internet网络，SICK_STREAM是TCP协议 
	
	struct sockaddr_in addr;	//定义一个IP地址结构
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;	//设置结构地址类型为TCP/IP地址
	addr.sin_port = htons(8080);	//指定一个端口号：8080，htons：将short类型从host字节类型转化为net字节类型 
	addr.sin_addr.s_addr = inet_addr("192.168.122.1");	//转化字符串，给addr结构
	
	if(connect(st,(struct sockaddr *)&addr,sizeof(addr)) == -1)
	{
		printf("connect faile,%s\n",strerror(errno));
		return 0;
	}
	char s[100];
	memset(s,0,sizeof(s));
	strcpy(s,"Hello world!");
	if(send(st,s,strlen(s),0) == -1)
	{
		printf("send faile,%s\n",strerror(errno));
		return 0;		
	} 
	return 0;
}
