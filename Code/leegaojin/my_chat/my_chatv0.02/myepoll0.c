#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<pthread.h>

#define SERV_PORT     8848 //服务器端口
#define LISTENQ       12    //链接请求队列的最大长度
int sock[2];

/*请求-数据包  包含 操作标记 发送方 接收方  信息
*/
typedef struct masg{
  int flag;
  char recv_mum[20]={0};
  char send_mem[20]={0};
  char data[1024]={0};
}MASG;
/************请求-数据包***************/

void setnonblocking(int st) //非阻塞
{
    int a = fcntl(st,F_GETFL);
    if(a < 0)
    {printf("change fcntl error\n");return;}
    a = a | O_NONBLOCK;
    if(fcntl(st,F_SETFL,a) < 0)
    {
        printf("set error\n");
        return;
    }
    return;
}

void my_err(const char* err_string,int line)
{
  fprintf(stderr,"line:%d ",line);
  perror(err_string);
  exit(1);
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
      if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in))<0)
      {
        my_err("bind",__LINE__);
      }


      //将套接字转化为监听套接字
    //  if(
        listen(sock_fd,LISTENQ);
        //<0);
    //  {
    //    my_err("listen",__LINE__);
    //  }
      cli_len = sizeof(struct sockaddr_in);
      printf("epoll初始化........\n");
      //printf("服务器sock_fd = %d\n",sock_fd);
      struct epoll_event ev,events[100];
      int epfd = epoll_create(100);
      setnonblocking(sock_fd);    //设置非阻塞
      ev.data.fd = sock_fd;
      ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
      epoll_ctl(epfd,EPOLL_CTL_ADD,sock_fd,&ev);
      int socket_c = 0;
                            int count = 0;
      while(1)
      {
        printf("等待用户操作.......\n");
        int nfds = epoll_wait(epfd,events,100,-1);
        printf("操作中........\n");
        if(nfds == -1)
        {
          printf("epoll_wait failed\n");
          break;
        }
        int i;
        for(i = 0;i<nfds;i++)
        {
          //printf("nfds=%d\n",nfds);
                if(events[i].data.fd<0)
                   continue;
                if(events[i].data.fd == sock_fd)
                {
                      socket_c = accept(sock_fd,(struct sockaddr*)&cli_addr,&cli_len);
                      printf("××××××××用户已链接,ip:%s\n",inet_ntoa(cli_addr.sin_addr));
                      /*
  ?  ///                将所有链接的套接字保存入一个数据库（在线用户）









                      */
                      printf("当前用户套接字socket = %d\n",socket_c);

                      sock[count]=socket_c;
                      count++;
                      //printf("count2 %d\n",count);
                      //printf("before socket[%d] = %d\n",--count,sock[--count]);    ////////////////////////////
                      printf("\n");
                      if(socket_c > 0)
                      {
                        setnonblocking(sock_fd);
                        ev.data.fd = socket_c;
                        ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                        epoll_ctl(epfd,EPOLL_CTL_ADD,socket_c,&ev);
                        continue;
                      }
                }
                if(events[i].events & EPOLLERR) //?????
                {
                  socket_c = events[i].data.fd;
                  close(socket_c);
                  epoll_ctl(epfd,EPOLL_CTL_DEL,socket_c,NULL);
                  continue;
                }
                if(events[i].events & EPOLLHUP)  //????
                {
                    socket_c = events[i].data.fd;
                    close(socket_c);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,socket_c,NULL);
                    continue;
                }
                if(events[i].events & EPOLLIN)
                {
                      socket_c = events[i].data.fd;
                      if(socket_c<=0)
                      {
                        close(socket_c);
                        epoll_ctl(epfd,EPOLL_CTL_DEL,socket_c,NULL);
                        continue;
                      }
                      char  recv_buf[1024];
                       //while(1){
                      if(recv(socket_c,recv_buf,sizeof(recv_buf),0)<0)
                      {
                        perror("recv");
                        exit(1);
                      }


                      printf("当前操作socket_c = %d\n",socket_c);
                      if(socket_c==sock[0])
                      send(sock[1],recv_buf,sizeof(recv_buf),0);
                      if(socket_c==sock[1])
                      send(sock[0],recv_buf,sizeof(recv_buf),0);
                      printf("客户端发送消息消息：%s\n",recv_buf);
                      memset(recv_buf,0,sizeof(recv_buf));
                      printf("用户操作结束.........[返回等待]\n");
                    //  }
                }
        }
      }
}
