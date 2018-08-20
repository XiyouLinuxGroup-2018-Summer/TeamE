#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>

#define CHAT_O 1 //私聊
#define REGISTER 2 //注册

#define PORT 8848
/*请求-数据包  包含 操作标记 发送方 接收方  信息
*/
typedef struct masg{
  int flag;
  char recv_mum[20]={0};
  char send_mem[20]={0};
  char data[1024]={0};
}MASG;
/************请求-数据包***************/
/*注册-登录-数据包 **/
typedef struct masg_rll{
  int flag;
  char name[20];
  char passwd[20];
  int status;
}
/**********注册登录数据包*********/
void meun_rll(void)
{
  puts("--------------------------------------------");
  puts("----------1.register 2.login----------------");
  puts("----------3.lost passwd --------------------");
  puts("--------------------------------------------");
}
/*注册函数
成功返回1 失败返回-1
参数为一个已连接的套接字   */
int reg(int socket)
{
     //输入flag
     //输入用户名
     //输入密码
     //发包
     //传回信息，是否成功
}
/*********************注册函数*****************/
void my_err(const char* err_string,int line)
{
  fprintf(stderr,"line:%d ",line);
  perror(err_string);
  exit(1);
}
void * show_client(void*arg)
{
  printf("-------------------正在聊天--------------------\n");
  char recv_buf[1024];
  int socket_c = *(int *)arg;
  while(1)
  {
    recv(socket_c,recv_buf,sizeof(recv_buf),0);
    printf("                                       %s\n",recv_buf);
  }
}
void* far_client(void*arg)
{
  char send_buf[1024];
  int socket_c = *(int*)arg;
  while(1)
  {
    //printf(" : ");
    int i;
    int c;
    i = 0;
    int len = 32;
    //memset(send_buf,0,sizeof(send_buf));
    while(((c=getchar())!='\n')&&(c!=EOF)&&(i<len-2))
    {
      send_buf[i++]=c;
    }
    //send_buf[i++]='\n';
    send_buf[i++]='\0';
    send(socket_c,send_buf,strlen(send_buf),0);
  }
}

int main(int argc,char*argv[])
{
  int socket_c;
  int serv_port;
  struct sockaddr_in serv_addr;
  int status;
  //char    recv_buf[1024];
  //初始化服务端的地址结构
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr =inet_addr(argv[1]);
  //创建一个套接字
  socket_c = socket(AF_INET,SOCK_STREAM,0);
  if(socket_c<0){
    my_err("socket",__LINE__);
  }
  if(connect(socket_c,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0){
    my_err("connect",__LINE__);
  }

  meun_rll()；    //打印第一层目录

  switch (1) {
    case CHAT_O:
    //简版私聊
    pthread_t id1,id2;
    int err = pthread_create(&id1,NULL,show_client,&socket_c);
    if(err != 0) printf("%d\n",errno);
    //pthread_detach(id1); // 线程分离

    pthread_create(&id2,NULL,far_client,&socket_c);
    //pthread_detach(id2); // 线程分离
    pthread_join(id1,(void *)&status);
    pthread_join(id2,(void *)&status);
    //sleep(3);
    break;
    case REGISTER:
    reg(socket_c); 
    //错误处理
    break;

  }


}
