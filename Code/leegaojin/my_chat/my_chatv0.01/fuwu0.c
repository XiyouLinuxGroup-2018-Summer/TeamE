#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>


#define SERV_PORT     8848 //服务器端口
#define LISTENQ       12    //链接请求队列的最大长度
int sock[2];
void *chuli_client(void*arg)
{
    int client_socket = *(int*)arg;
    char  recv_buf[1024];
    while(1){
      int rec = recv(client_socket,recv_buf,sizeof(recv_buf),0);
      if(rec <0){
        perror("recv");
      //  exit(1);
    }else if(rec == 0){
      printf("xxx已下线......");//xxx已下线  发送给对方
      pthread_exit(0);   //退出线程
    }
      if(client_socket==sock[0])
      send(sock[1],recv_buf,sizeof(recv_buf),0);
      if(client_socket==sock[1])
      send(sock[0],recv_buf,sizeof(recv_buf),0);
      printf("%s\n",recv_buf);
    }
}

int main()
{
  int   sock_fd,conn_fd[5];
  int   i=0;
  int   optval;
  pid_t pid;
  socklen_t cli_len;
  struct sockaddr_in  cli_addr,serv_addr;
  char  recv_buf[128];

  //创建一个tcp套接字
  sock_fd = socket(AF_INET,SOCK_STREAM,0);
  //设置该套接字让它可以重新绑定端口
  optval =1;
  setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int));

  //初始化服务器端地址结构
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  //将套接字绑定到本地端口
  bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in));


  //将套接字转化为监听套接字
  //if(
  listen(sock_fd,LISTENQ);
  //  <0);
  //  {
  //  my_err("listen",__LINE__);
  ///}
  cli_len = sizeof(struct sockaddr_in);
  printf("等待用户链接.........");

  while(1)
  {
    int client_socket = accept(sock_fd,(struct sockaddr*)&cli_addr,&cli_len);
    sock[i++]=client_socket;
    printf("accept a new client,ip:%s\n",inet_ntoa(cli_addr.sin_addr));
    //创建一个子线程处理刚刚接收的连接请求
    pthread_t id;
    pthread_create(&id,NULL,chuli_client,&client_socket);

    pthread_detach(id); // 线程分离
  }

}
