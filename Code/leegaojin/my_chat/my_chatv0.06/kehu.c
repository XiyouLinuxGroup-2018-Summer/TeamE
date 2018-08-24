#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define CHAT_O 4	   //私聊功能
#define REGISTER 1	 // 注册
#define LOGIN 2		   //  登录
#define QUIT 3		   //退出  //用户下线
#define ADD_FRI 5	  //添加好友
#define DEL_FRI 6	  //删除好友
#define DIS_FRI 7	  //显示(刷新)好友列表
#define CRE_GRO 8  //创建一个群
#define DIS_GRO 9   //显示(刷新)群列表
#define CHAT_GRO 10  //群聊


#define REG_SUC 10001	  //注册成功
#define LOG_SUC 10002	  //登陆成功
#define LOG_FAU 10003	  //登录失败
#define ADD_FRI_SCU 10004  //添加好友成功
#define ADD_FRI_FAU 10005  //添加好友失败
#define ACC_ADD 10006	  //好友请求
#define DEL_FRI_MASG 10007 //删除好友通知
#define DEL_FRI_SCU 10008  //删除成功通知
#define MASG_ACC 10009	 //消息提示
#define OUT_ROOM 10010	 //离开聊天室通知
#define A_MESS 10011	   //保存为普通消息
#define OFF_MESS 10012	 //保存为未读消息
//#define DIS_MES_HISS 10013 //读自己的历史消息(美观需求)
#define DIS_MES_ON 10014   //显示未读消息
#define DIS_MES_OFF 10015  //读取未读消息
#define DIS_MES_HIS 10016 //读取历史记录
#define CRE_GRO_SUC 10017 //创建群成功
#define INV_MEM      10018 //邀请进群
#define DIS_OFF_MES  10019 //提示未读群通知//第一层通知
#define DIS_ON_MES  10020 //查看未读群通知 //群聊界面
#define A_GRO_MESS 10021 //保存为普通群消息
#define OFF_GRO_MESS 10022 //保存为未读群消息
#define DIS_GRO_MEM  10023 //显示群成员
int room_status = 0;

#define PORT 8848
/*请求-数据包  包含 操作标记 发送方 接收方  信息
*/
typedef struct masg
{
	int flag;
	char recv_mem[20];
	char send_mem[20];
	char group[20];
	char data[1024];
} MASG;
/***********请求-数据包**************/
/*退出函数**********/
void quit(MASG *masg, int socket, int flag)
{
	masg->flag = flag;
	send(socket, masg, sizeof(MASG), 0);
}
/************/
/***************个人界面******************/
void meun_per(void)
{
	puts(">我的好友                                9.注销登录");
	puts("-------------------------------------------------------");
	puts("|1.添加好友       2.删除好友                          |");
	puts("|0.私聊          3.显示(刷新)好友列表                 |");
	puts("-------------------------------------------------------");
	puts(">我的群");
	puts("-------------------------------------------------------");
	puts("|4.创建群    5.显示(刷新)群列表  6.进入群聊 |");
	puts("|               ");
	puts("-------------------------------------------------------");

}
/*************群聊******************/
void chat_group(MASG*masg,int socket)
{
	printf("请输入要进入的群名：");
	scanf("%s",masg->group);
	masg->flag = CHAT_GRO;

	printf("+---h to history----群聊：%s-----------q to quit-+\n", masg->group);
	puts("+-'*'邀请进群-+");
	masg->flag = DIS_GRO_MEM;   //显示群成员
	send(socket,masg,sizeof(MASG),0);  
	//masg->flag = DIS_MES_OFF;    //读取未读消息
	//send(socket,masg,sizeof(MASG),0);
	masg->flag = CHAT_GRO;
	room_status = 1;
	while (1)
	{
		scanf("%s", masg->data);
		getchar();
		if (strcmp(masg->data, "q") == 0)
		{
			//masg->flag = OUT_ROOM;   //退出聊天室通知
			//send(socket, masg, sizeof(MASG), 0);
			break;
		}
		if(strcmp(masg->data,"h")==0)
		{
			//masg->flag = DIS_MES_HIS;  //获取群聊天记录
			//send(socket,masg,sizeof(MASG),0);
			//masg->flag = CHAT_O;
	        //continue;
		}
		if(strcmp(masg->data,"*")==0)
		{
            printf("被邀请人：");
			scanf("%s",masg->recv_mem);
			masg->flag = INV_MEM;
			send(socket,masg,sizeof(MASG),0);
			masg->flag = CHAT_GRO;
			continue;
		}
		send(socket, masg, sizeof(MASG), 0);
	}
	room_status = 0;
	return;
}

/***********显示(刷新)群列表**********/
void dis_group(MASG*masg,int socket)
{
	masg->flag = DIS_GRO;
	send(socket,masg,sizeof(MASG),0);
}
/************创建群********************/
void create_group(MASG*masg,int socket)
{
	printf("请输入想要创建的群名:");
	scanf("%s",masg->data);
	masg->flag = CRE_GRO;
	send(socket,masg,sizeof(MASG),0);
}
/***********私聊*************************/
void chat_only(MASG *masg, int socket)
{
	printf("请选择聊天对象(name):");
	scanf("%s", masg->recv_mem);
	masg->flag = CHAT_O;

	printf("+---h to history----正在与%s聊天-----------q to quit-+\n", masg->recv_mem);
	masg->flag = DIS_MES_OFF;
	send(socket,masg,sizeof(MASG),0);
	masg->flag = CHAT_O;
	room_status = 1;
	while (1)
	{
		scanf("%s", masg->data);
		getchar();
		if (strcmp(masg->data, "q") == 0)
		{
			masg->flag = OUT_ROOM;
			send(socket, masg, sizeof(MASG), 0);
			break;
		}
		if(strcmp(masg->data,"h")==0)
		{
			masg->flag = DIS_MES_HIS;
			send(socket,masg,sizeof(MASG),0);
			masg->flag = CHAT_O;
	        continue;
		}
		send(socket, masg, sizeof(MASG), 0);
	}
	room_status = 0;
	return;
}
/***********显示(刷新)好友列表***********/
void dis_fir_list(MASG *masg, int socket)
{
	masg->flag = DIS_FRI;
	send(socket, masg, sizeof(MASG), 0);
}

/************删除好友*******************/
void del_fri(MASG *masg, int socket)
{
	printf("请输入你想要删除的好友:");
	scanf("%s", masg->recv_mem);
	masg->flag = DEL_FRI;
	send(socket, masg, sizeof(MASG), 0);
}
/*****************************/

/**********加好友******/
void add_fri(MASG *masg, int socket)
{
	printf("send_mem=%s\n", masg->send_mem);
	printf("flag = %d\n", masg->flag);
	printf("请输入你想要添加的好友（名字）：");
	scanf("%s", masg->recv_mem);
	masg->flag = ADD_FRI;
	printf("after flag = %d\n", masg->flag);
	send(socket, masg, sizeof(MASG), 0);
}

/***********个人界面*******************/
void per(MASG *masg, int socket)
{
	while (1)
	{
		meun_per();
		masg->flag = DIS_MES_ON;
		send(socket,masg,sizeof(MASG),0);
		int flag = 0;
		scanf("%d", &flag);
		switch (flag + 4)
		{
		case ADD_FRI:
			add_fri(masg, socket);
			break;
		case DEL_FRI:
			del_fri(masg, socket);
			break;
		case 13:
			quit(masg, socket, 3);
			return;
		case DIS_FRI:
			dis_fir_list(masg, socket);
			break;
		case CHAT_O:
			chat_only(masg, socket);
			break;
		case CRE_GRO:
			create_group(masg,socket);
			break;
		case DIS_GRO:
		    dis_group(masg,socket);
			break;
		case CHAT_GRO:
			chat_group(masg,socket);
			break;
		}
	}
}
/**************************************/

/*****************登录注册界面**************/
void meun_rll(void)
{
	puts("          welcome  to  my_chat              ");
	puts("--------------------------------------------");
	puts("|         1.register 2.login               |");
	puts("|         3.exit                           |");
	puts("--------------------------------------------");
	puts("                            my_chat v0.06  ");
}
/********************************/

/*********
注册函数
成功返回1 失败返回-1
参数为一个已连接的套接字*/
int login(int socket, int flag)
{
	MASG masg;
	printf("**************login**********\n");
	masg.flag = flag;
	//输入flag
	printf("请输入用户名：");
	scanf("%s", masg.send_mem);
	getchar();
	//输入用户名
	printf("请输入密码：");
	scanf("%s", masg.data);
	getchar();
	//输入密码

	send(socket, &masg, sizeof(MASG), 0);
	//发包
	//传回信息，是否成功 //交给主接收函数;
	printf("******************log_end\n");
	per(&masg, socket); //进入个人界面
}
int reg(int socket, int flag)
{
	MASG masg;
	printf("************register************\n");

	masg.flag = flag;
	//输入flag
	printf("请输入用户名：");
	scanf("%s", masg.send_mem);
	getchar();
	//输入用户名
	printf("请输入密码：");
	scanf("%s", masg.data);
	getchar();
	//输入密码

	send(socket, &masg, sizeof(MASG), 0);
	//发包
	//传回信息，是否成功 //交给主接收函数
	printf("**************************reg_end*\n");
}
/*********************注册函数*****************/

void my_err(const char *err_string, int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}

void *show_client(void *arg)
{
	MASG masg;
	int socket_c = *(int *)arg;
	while (1)
	{
		recv(socket_c, &masg, sizeof(MASG), 0);
		//  printf(".%s\n",masg.data);
		 //printf("return number = %d\n",masg.flag);
		int flag = masg.flag;
		switch (flag)
		{
		case REG_SUC:
			printf("注册成功！\n");
			break;
		case LOG_SUC:
			printf("登陆成功!\n");
			break;
		case LOG_FAU:
			printf("账号或密码不存在，请重新输入!\n");
			break;
		case QUIT:
			printf("%s 已注销\n", masg.send_mem);
			//pthread_exit(0);                      //修改
			break;
		case ADD_FRI_SCU:
			printf("%s 已接受了你的添加请求\n", masg.recv_mem);
			break;
		case ADD_FRI_FAU:
			printf("对方拒绝了你的好友请求\n");
			break;
		case ACC_ADD:
			printf("%s 向你发来一个好友请求\n", masg.send_mem);
			char acc;
			printf("是否同意(y/n)\n");
			scanf("%c", &acc);
			getchar();
			if (acc == 'y')
			{
				masg.flag = ADD_FRI_SCU;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			else if (acc == 'n')
			{
				masg.flag = ADD_FRI_FAU;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			break;

		case DEL_FRI_MASG:
			printf("%s 将你移出友好友列表\n", masg.send_mem);
			break;
		case DEL_FRI_SCU:
			printf("你已经成功将 %s 移出好友\n", masg.recv_mem);
			break;
		case DIS_FRI:
			printf("%s\n", masg.data);
			break;
		case CHAT_O:
			if (room_status)
			{
				printf("                                      %s:%s\n", masg.send_mem, masg.data);
				masg.flag = A_MESS;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			else if (!room_status)
			{
				printf("%s 给你发来一条消息\n", masg.send_mem);
				masg.flag = OFF_MESS;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			break;
		case OUT_ROOM:
			if (room_status)
				printf("对方已退出聊天!(可接受)\n");
			break;
		case DIS_MES_ON:
		    printf("有一条来自 %s 的消息\n",masg.recv_mem);
			break;
		case DIS_MES_OFF:
		    printf("                                     %s:%s\n",masg.recv_mem,masg.data);
			break;
		case DIS_MES_HIS:
		    printf("%s:%s\n",masg.send_mem,masg.data);
			break;
        //case DIS_MES_HISS:
		//	printf("me:%s",masg.data);
		//	break;
		case CRE_GRO_SUC:
			printf("群创建成功！\n");
			break;
		case DIS_GRO :
		    printf("%s\n",masg.data);
			break;
		case INV_MEM:
			printf("%s 将你拉入群聊 %s\n",masg.send_mem,masg.group);
			break;
		case CHAT_GRO:
			if (room_status)
			{
				printf("                                      %s:%s\n", masg.send_mem, masg.data);
				masg.flag = A_GRO_MESS;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			else if (!room_status)
			{
				printf("%s 群有一条新消息\n", masg.group);
				masg.flag = OFF_GRO_MESS;
				send(socket_c, &masg, sizeof(MASG), 0);
			}
			break;
		case DIS_GRO_MEM:
			printf("|%s|\n",masg.data);
			break;
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
int main(int argc, char *argv[])
{
	int socket_c;
	int serv_port;
	struct sockaddr_in serv_addr;
	int status;
	int flag;
	//char    recv_buf[1024];
	//初始化服务端的地址结构
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	//创建一个套接字
	socket_c = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_c < 0)
	{
		my_err("socket", __LINE__);
	}
	if (connect(socket_c, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		my_err("connect", __LINE__);
	}

	pthread_t id1, id2;
	int err = pthread_create(&id1, NULL, show_client, &socket_c);
	if (err != 0)
		printf("%d\n", errno);

	//pthread_create(&id2,NULL,far_client,&socket_c);
	//pthread_join(id2,(void *)&status);
	while (1)
	{
		meun_rll(); //打印第一层目录

		//printf("请输入选项:");
		scanf("%d", &flag);

		switch (flag)
		{
		case REGISTER:
			reg(socket_c, flag);
			break;
		case LOGIN:
			login(socket_c, flag);
			break;
		case QUIT:
			return 0;
		}
	}

	pthread_join(id1, (void *)&status);
}
