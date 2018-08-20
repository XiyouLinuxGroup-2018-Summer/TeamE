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
#include<mysql/mysql.h>

#define CHAT_O     4//私聊功能
#define REGISTER   1 // 注册
#define LOGIN      2 //  登录
#define QUIT       3 //退出  //用户下线

#define REG_SUC    10001 //注册成功
#define LOG_SUC    10002 //登陆成功
#define LOG_FAU    10003 //登录失败

#define SERV_PORT     8848 //服务器端口
#define LISTENQ       12    //链接请求队列的最大长度
int sock[2];

/*请求-数据包  包含 操作标记 发送方 接收方  信息
*/
typedef struct masg{
  int flag;
  char recv_mum[20];
  char send_mem[20];
  char data[1024];
}MASG;
/************请求-数据包***************/

/*清空数据包
void del_pack (MASG*masg)
{
   masg->flag = 0;
   memset(masg->recv_num,0,sizeof(masg->recv_num))
   memset(masg->send_mem,0,sizeof(masg->send_mem))
   memset(masg->data,0,sizeof(masg->data))
}
/********************/
/****退出函数********
******/
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
      MASG masg;           //声明结构体
/*数据库 初始化 链接****/
      MYSQL mysql;
    	//初始化MYSQL句柄
    	if(NULL == mysql_init(&mysql)){
    		printf("init: %s\n",mysql_error(&mysql));
    	}

    	//建立MYSQL连接

    	if(NULL == mysql_real_connect(&mysql,"localhost","debian-sys-maint","ZCoY8eh3ichrX4X0","client_system",0,NULL,0)){
    		printf("connect: %s\n",mysql_error(&mysql));
    	}
/******************/


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
                printf("*********用户已链接,ip:%s\n",inet_ntoa(cli_addr.sin_addr));
                printf("当前用户套接字socket = %d\n",socket_c);
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

          if(events[i].events & EPOLLIN)    //遇到事件！！！
          {
          socket_c = events[i].data.fd;
          if(socket_c<=0)
          {
            close(socket_c);
            epoll_ctl(epfd,EPOLL_CTL_DEL,socket_c,NULL);
            continue;
          }
          //char  recv_buf[1024];
          if(recv(socket_c,&masg,sizeof(MASG),0)<0)
          {
            perror("recv");
            exit(1);
          }

          printf("当前操作socket_c = %d\n",socket_c);
          //解包   查找选项

          int flag = masg.flag; //获取消息类型
          printf("获取消息类型为:%d\n",flag);
          

            switch (flag) 
            {
              case CHAT_O :
              printf("........私聊操作...");
                if(socket_c==sock[0])
                send(sock[1],masg.data,sizeof(masg.data),0);
                if(socket_c==sock[1])
                send(sock[0],masg.data,sizeof(masg.data),0);
                printf("客户端发送消息：%s\n",masg.data);

                memset(&masg,0,sizeof(MASG));
                printf("用户操作结束.........[返回等待]\n\n");
                break;


              case REGISTER :

              printf(".........注册.........\n");
              //执行插入语句
              char* sql=NULL;
              sql = (char*)calloc(1024,sizeof(char));
              sprintf(sql,"INSERT INTO client_member(name,passwd) VALUES('%s','%s')",masg.send_mem,masg.data);
              if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
                printf("query: %s\n",mysql_error(&mysql));
              }
              
              masg.flag = REG_SUC;
              send(socket_c,&masg,sizeof(MASG),0);
              //执行查询语句
              sql = "SELECT id,name,passwd FROM client_member";
              if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
                printf("query: %s\n",mysql_error(&mysql));
              }
              MYSQL_RES *res = mysql_store_result(&mysql);
              MYSQL_ROW row;
              while ((row = mysql_fetch_row(res))) {
                printf("%s %s %s\n",row[0],row[1],row[2]);
              }
              //memset(sql,0,sizeof(sql));
              //mysql_free_result(res);
              //mysql_close(&mysql);
                break;

              case LOGIN:

              sprintf(sql,"SELECT * FROM client_member where name='%s' and passwd='%s'",masg.send_mem,masg.data);
              if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
                printf("query: %s\n",mysql_error(&mysql));
              }
              res = mysql_store_result(&mysql);

              if ((row = mysql_fetch_row(res))) {
                
                printf("%s 登陆成功\n",masg.send_mem);
                printf("%s %s %s\n",row[0],row[1],row[2]);
              ///////注册进在线列表
              sprintf(sql,"INSERT INTO client_online (socket,name) VALUES('%d','%s')",socket_c,masg.send_mem);
              if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
                printf("query: %s\n",mysql_error(&mysql));
              }
              printf("%s 加入在线列表\n",masg.send_mem);
              //////////////////

                masg.flag = LOG_SUC;
                send(socket_c,&masg,sizeof(MASG),0);
              }
              else if ((!mysql_fetch_row(res)))
              {
                printf("登录失败！\n");
                masg.flag = LOG_FAU;
                send(socket_c,&masg,sizeof(MASG),0);
              }
              break;

              case QUIT:

              printf("%s 已下线！\n",masg.send_mem);
              //从在线表中删除
              sprintf(sql,"DELETE FROM client_online WHERE name ='%s'",masg.send_mem);
              printf("从在线用户表中删除..\n");
              if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
                printf("query: %s\n",mysql_error(&mysql));
              }
              masg.flag = QUIT;
              send(socket_c,&masg,sizeof(MASG),0);
              break;

                      



            }//switch }

          }//事件 }
         //  printf("轮寻.......\n");
        }//for }
      //  printf("while........\n");
      } //while }
    //  printf("服务器关闭.......\n");
}
