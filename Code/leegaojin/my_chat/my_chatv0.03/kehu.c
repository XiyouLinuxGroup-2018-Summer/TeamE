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

#define CHAT_O     4//私聊功能
#define REGISTER   1 // 注册
#define LOGIN      2 //  登录
#define QUIT       3 //退出  //用户下线

#define REG_SUC    10001 //注册成功
#define LOG_SUC    10002 //登陆成功
#define LOG_FAU    10003 //登录失败

#define PORT 8848
/*请求-数据包  包含 操作标记 发送方 接收方  信息
*/
typedef struct masg{
  int flag;
  char recv_mum[20];
  char send_mem[20];
  char data[1024];
}MASG;
/***********请求-数据包**************/

void meun_rll(void)
{
  puts("          welcome  to  my_chat              ");
  puts("--------------------------------------------");
  puts("|         1.register 2.login               |");
  puts("|         3.exit                           |");
  puts("--------------------------------------------");
  puts("                            my_chat v0.03  ");
}
/*退出函数**********/
void quit (int socket,int flag)
{
  MASG masg;
  masg.flag = flag;
  send(socket,&masg,sizeof(MASG),0);
}
/************/
/*********
注册函数
成功返回1 失败返回-1
参数为一个已连接的套接字*/
int login(int socket,int flag)
{
  MASG masg;
  printf("**************login**********\n");
    masg.flag = flag;
    //输入flag
    printf("请输入用户名：");
    scanf("%s",masg.send_mem);
    getchar();
    //输入用户名
    printf("请输入密码：");
    scanf("%s",masg.data);
    getchar();
    //输入密码

  send(socket,&masg,sizeof(MASG),0);
    //发包
    //传回信息，是否成功 //交给主接收函数;
  printf("******************log_end\n");
}
int reg(int socket,int flag)
{
  MASG masg;
  printf("************register************\n");

     masg.flag = flag;
     //输入flag
     printf("请输入用户名：");
     scanf("%s",masg.send_mem);
     getchar();
     //输入用户名
     printf("请输入密码：");
     scanf("%s",masg.data);
     getchar();
     //输入密码

    send(socket,&masg,sizeof(MASG),0);
     //发包
     //传回信息，是否成功 //交给主接收函数
  printf("**************************reg_end*\n");

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
  MASG masg;
  int socket_c = *(int *)arg;
  while(1)
  {
    recv(socket_c,&masg,sizeof(MASG),0);
    printf(".%s\n",masg.data);
    printf("return number = %d\n",masg.flag);
    int flag = masg.flag;

    switch(flag){
      case REG_SUC:
      printf("注册成功！\n");
      break;
      case LOG_SUC:
      printf("登陆成功!\n");
      break;
      case LOG_FAU:
      printf("账号或密码不存在，请重新输入!\n");
      break;
      case QUIT :
      printf("我在退出\n");
      pthread_exit(0);
      default:
      printf("errno!\n");
      break;
    }
  }
}
/*
void* far_client(void*arg)
{
  MASG masg;

  int socket_c = *(int*)arg;
   printf("请输入你要操作的数字：");
   scanf("%d",&masg.flag);
   getchar();
   switch (masg.flag)
   {
   c
   }
}
*/
int main(int argc,char*argv[])
{
  int socket_c;
  int serv_port;
  struct sockaddr_in serv_addr;
  int status;
  int flag;
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


    pthread_t id1,id2;
    int err = pthread_create(&id1,NULL,show_client,&socket_c);
    if(err != 0) printf("%d\n",errno);

    //pthread_create(&id2,NULL,far_client,&socket_c);
    //pthread_join(id2,(void *)&status);
while(1)
{
    meun_rll();    //打印第一层目录
    
    //printf("请输入选项:");
    scanf("%d",&flag);

    switch(flag)
    {
      case REGISTER :
      reg(socket_c,flag);
      break;
      case LOGIN:
      login(socket_c,flag);
      break;
      case QUIT :
      quit(socket_c,flag);
      return 0;


    }
}

    pthread_join(id1,(void *)&status);
}
