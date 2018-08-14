#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

void my_err(char *string, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(string);
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	my_err("intput erro", __LINE__);
	int sockfd;
	int portnumber;
	struct sockaddr_in cli_addr, serv_addr;
	char readbuf[1024] = {0};
	char wbuf[1024] = {0};
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("sockfd", __LINE__);
	portnumber = atoi(argv[1]);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnumber);
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
	socklen_t len = sizeof(struct sockaddr);
	if(connect(sockfd, (struct sockaddr*)&serv_addr, len) < 0)
		my_err("connect", __LINE__);
	while(1)
	{
		if(recv(sockfd, readbuf, 1024, 0) < 0)
			my_err("recv", __LINE__);
		readbuf[strlen(readbuf)] = '\0';
		if(strcmp(readbuf, "q") == 0)
			break;
		printf("serve:%s\n", readbuf);
		fgets(wbuf, 1024, stdin);
		wbuf[strlen(wbuf) - 1] = '\0';
		if(send(sockfd, wbuf, strlen(wbuf) + 1, 0) < 0)
			my_err("send", __LINE__);
	}
	close(sockfd);
}
