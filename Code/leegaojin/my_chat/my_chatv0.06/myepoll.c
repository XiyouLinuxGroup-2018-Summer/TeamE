#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <mysql/mysql.h>

#define REGISTER 1	 // 注册
#define LOGIN 2		   //  登录
#define QUIT 3		   //退出  //用户下线
#define ADD_FRI 5	  // 添加好友
#define DEL_FRI 6	  // 删除好友
#define DIS_FRI 7	  //显示(刷新)好友列表
#define CHAT_O 4	   //私聊功能
#define CRE_GRO 8 //创建群
#define DIS_GRO 9 //显示(刷新)群列表
#define CHAT_GRO 10 //群聊

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
#define DIS_MES_HISS 10013 //读取自己的历史消息(美观需求)
#define DIS_MES_ON 10014   //显示未读消息
#define DIS_MES_OFF 10015  //读取未读消息
#define DIS_MES_HIS 10016 //读取历史记录
#define CRE_GRO_SUC 10017 //创建群成功
#define INV_MEM 10018 //邀请进群
#define DIS_OFF_MES  10019 //提示未读群通知//第一层通知
#define DIS_ON_MES  10020 //查看未读群通知 //群聊界面
#define A_GRO_MESS 10021 //保存为普通群消息
#define OFF_GRO_MESS 10022 //保存为未读群消息
#define DIS_GRO_MEM  10023 //显示群成员

#define SERV_PORT 8848 //服务器端口
#define LISTENQ 12	 //链接请求队列的最大长度

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
/************请求-数据包***************/
/************查找当前在线用户名套接字***********/
/*参数1为名字，3为数据库句柄   返回值为当前套接字,没有找到返回0*/
int select_c(char *name, MYSQL *mysql)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM client_online where name='%s'", name);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);

	if ((row = mysql_fetch_row(res)))
	{

		//printf("%s 发送**请求成功\n",masg->send_mem);
		printf("%s %s %s\n", row[0], row[1], row[2]);
		int socket_send = (int)row[1][0] - '0';
		return socket_send;
	}
	else if ((!mysql_fetch_row(res)))
	{
		printf("获取在线套接字失败，%s不在线\n", name);
		return 0;
	}
}
/***********显示群成员********/
void dis_group_member(MYSQL*mysql,MASG*masg,int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	int socket_send = 0;
	sql = (char *)calloc(1024, sizeof(char));

	sprintf(sql, "SELECT * FROM group_list where group_name ='%s'", masg->group);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);

	while ((row = mysql_fetch_row(res)))
	{
		printf("显示群成员\n");
		printf("%s %s %s\n", row[0], row[1], row[2]);
		strcpy(masg->data,row[2]);
		send(socket,masg,sizeof(MASG),0);
	}

}
/***********群聊*************/
void chat_group (MYSQL*mysql,MASG*masg,int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	int socket_send = 0;
	sql = (char *)calloc(1024, sizeof(char));

	sprintf(sql, "SELECT * FROM group_list where group_name ='%s'", masg->group);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);

	while ((row = mysql_fetch_row(res)))
	{
		printf("%s %s %s\n", row[0], row[1], row[2]);
		if(strcmp(masg->send_mem,row[2])==0)
		continue;
		printf("%s 群聊发起者..\n");
		socket_send = select_c(row[2],mysql);
		strcpy(masg->recv_mem,row[2]);
		if(socket_send)
		{
		send(socket_send, masg, sizeof(MASG), 0);
		}
		else if (!socket_send)
		{
			//保存为群离线消息
			sprintf(sql, "INSERT INTO group_message (send,recv,qun,message,status) VALUES('%s','%s','%s','%s',2)", masg->send_mem, masg->recv_mem, masg->group,masg->data);
			if (0 != mysql_real_query(mysql, sql, strlen(sql)))
			{
				printf("query: %s\n", mysql_error(mysql));
			}
			printf("消息已加入离线群消息列表!\n");
		}
	}

}
/************邀请进群******************/
void invite_to_group(MYSQL*mysql,MASG*masg,int socket)
{
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	printf("正在加入群聊列表...\n");
	sprintf(sql, "INSERT INTO group_list (group_name,member) VALUES('%s','%s')", masg->group, masg->recv_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	printf("消息已加入离线群通知!\n");
	int socket_send = select_c(masg->recv_mem, mysql);
	if (socket_send)
	{
		send(socket_send, masg, sizeof(MASG), 0);
	}
	else if (!socket_send)
	{
		//保存为离线群通知
		sprintf(sql, "INSERT INTO group_message (send,recv,qun,message,status) VALUES('%s','%s','%s','tips',3)", masg->send_mem, masg->recv_mem, masg->group);
		if (0 != mysql_real_query(mysql, sql, strlen(sql)))
		{
			printf("query: %s\n", mysql_error(mysql));
		}
		printf("消息已加入离线群通知!\n");
	}

}
/***********显示(刷新)群列表**************/
void dis_group (MYSQL*mysql,MASG*masg,int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM group_list where member ='%s'", masg->send_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);
	int status = 0;
	while ((row = mysql_fetch_row(res)))
	{
		printf("%s %s %s\n", row[0], row[1], row[2]);
		strcpy(masg->data, row[1]);
		send(socket, masg, sizeof(MASG), 0);
	}

}
/************创建群***************/
void create_group (MYSQL*mysql,MASG*masg,int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	printf("正在建群.....\n");
	sprintf(sql, "INSERT INTO group_list(group_name,member) VALUES('%s','%s')", masg->data,masg->send_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	masg->flag = CRE_GRO_SUC;
	send(socket,masg,sizeof(MASG),0);
}
/***********获取聊天记录***********/
void dis_mes_his(MYSQL*mysql,MASG*masg,int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM message_history  WHERE recv ='%s'and send = '%s' OR recv = '%s' and send = '%s' ", masg->send_mem,masg->recv_mem,masg->recv_mem,masg->send_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);
	while((row = mysql_fetch_row(res)))
	{
		printf("%s %s %s %s %s\n", row[0], row[1], row[2],row[3],row[4]);
               //if(strcmp(masg->recv_mem,row[1])==0&& strcmp(masg->send_mem,row[2])==0)
			   //{
				printf("当前状态，status = %s\n",row[4]);
                  strcpy(masg->data,row[3]);
				  strcpy(masg->send_mem,row[1]);
				  send(socket,masg,sizeof(MASG),0);   //发送历史消息
				  printf("正在发送历史记录!\n");
			   //}
               //if(strcmp(masg->send_mem,row[1])==0&& strcmp(masg->recv_mem,row[2])==0)
			   //{
			//	printf("当前状态，status = %s\n",row[4]);
            //      strcpy(masg->data,row[3]);
			//	  masg->flag = DIS_MES_HISS;
			//	  send(socket,masg,sizeof(MASG),0);   //发送历史消息
			//	  printf("正在发送历史记录!\n");
			//   }

	}

}
/***********查看未读消息********/
void dis_mes_on(MYSQL *mysql, MASG *masg, int socket)
{ //分为显示未读，读取未读，获取聊天记录
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM message_history  where recv ='%s'", masg->send_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);
	int flag = 0;
	while ((row = mysql_fetch_row(res)))
	{
		printf("%s %s %s %s %s\n", row[0], row[1], row[2],row[3],row[4]);
		strcpy(masg->data, row[4]);
        flag = row[4][0]-'0';
		printf("flag = %d\n",flag);
		if (flag == 1 || flag == 2 ) 
		{
			if (masg->flag == DIS_MES_ON) //显示未读消息
			{
			  strcpy(masg->recv_mem,row[1]);
              send(socket,masg,sizeof(MASG),0);
			  printf("发送消息提示\n");
			}
			else if (masg->flag == DIS_MES_OFF) //读取未读消息
			{
				printf(".....\n");
               if(strcmp(masg->recv_mem,row[1])==0&& strcmp(masg->send_mem,row[2])==0)
			   {
				printf("当前状态，status = %s\n",row[4]);
                  strcpy(masg->data,row[3]);
				  strcat(masg->data,"(未读)");
				  send(socket,masg,sizeof(MASG),0);   //发送未读消息
				  printf("未读消息已发送!\n");
					sprintf(sql, "UPDATE message_history SET status = 0 WHERE send ='%s' and recv = '%s'", masg->recv_mem,masg->send_mem);
					if (0 != mysql_real_query(mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(mysql));
					}
					//没有 select printf("状态已更改，status = %s\n",row[4]);
				 

			   }
			}

		}
/*		if(masg->flag == DIS_MES_HIS) //显示历史记录
		{
               if(strcmp(masg->recv_mem,row[1])==0&& strcmp(masg->send_mem,row[2])==0)
			   {
				printf("当前状态，status = %s\n",row[4]);
                  strcpy(masg->data,row[3]);
				  send(socket,masg,sizeof(MASG),0);   //发送历史消息
				  printf("正在发送历史记录!\n");
			   }
               if(strcmp(masg->send_mem,row[1])==0&& strcmp(masg->recv_mem,row[2])==0)
			   {
				printf("当前状态，status = %s\n",row[4]);
                  strcpy(masg->data,row[3]);

				  send(socket,masg,sizeof(MASG),0);   //发送历史消息
				  printf("正在发送历史记录!\n");
			   }


		}
		*/
	}
}
/***********私聊******************/
void chat_only(MYSQL *mysql, MASG *masg, int socket)
{
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	int socket_send = select_c(masg->recv_mem, mysql);
	if (socket_send)
	{
		send(socket_send, masg, sizeof(MASG), 0);
	}
	else if (!socket_send)
	{
		//保存为离线消息
		sprintf(sql, "INSERT INTO message_history (send,recv,message,status) VALUES('%s','%s','%s',2)", masg->send_mem, masg->recv_mem, masg->data);
		if (0 != mysql_real_query(mysql, sql, strlen(sql)))
		{
			printf("query: %s\n", mysql_error(mysql));
		}
		printf("消息已加入离线列表!\n");
	}
}
/***********显示(刷新)好友列表*******/
void dis_fri_list(MYSQL *mysql, MASG *masg, int socket)
{

	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM client_friend where name_a ='%s'", masg->send_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);
	int status = 0;
	while ((row = mysql_fetch_row(res)))
	{
		printf("%s %s %s\n", row[0], row[1], row[2]);
		strcpy(masg->data, row[2]);
		status = select_c(masg->data, mysql);
		if (status)
		{
			strcat(masg->data, "(在线)");
		}
		else if (!status)
		{
			strcat(masg->data, "(离线)");
		}
		send(socket, masg, sizeof(MASG), 0);
	}
}
/***********添加好友************/
void add_fri(MYSQL *mysql, MASG *masg, int socket)
{
	MYSQL_RES *res = mysql_store_result(mysql);
	MYSQL_ROW row;
	char *sql = NULL;
	sql = (char *)calloc(1024, sizeof(char));
	sprintf(sql, "SELECT * FROM client_online where name='%s'", masg->recv_mem);
	if (0 != mysql_real_query(mysql, sql, strlen(sql)))
	{
		printf("query: %s\n", mysql_error(mysql));
	}
	res = mysql_store_result(mysql);

	if ((row = mysql_fetch_row(res)))
	{

		printf("%s 发送好友请求成功\n", masg->send_mem);
		printf("%s %s %s\n", row[0], row[1], row[2]);
		int socket_send = (int)row[1][0] - '0';
		masg->flag = ACC_ADD;
		printf("%d\n", socket_send);
		send(socket_send, masg, sizeof(MASG), 0);
	}
	else if ((!mysql_fetch_row(res)))
	{
		printf("添加失败！(对方不在线)\n");
		masg->flag = ADD_FRI_FAU;
		send(socket, &masg, sizeof(MASG), 0);
	}
}

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
	int a = fcntl(st, F_GETFL);
	if (a < 0)
	{
		printf("change fcntl error\n");
		return;
	}
	a = a | O_NONBLOCK;
	if (fcntl(st, F_SETFL, a) < 0)
	{
		printf("set error\n");
		return;
	}
	return;
}

void my_err(const char *err_string, int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}
int main()
{
	int sock_fd, conn_fd[5];
	int i = 0;
	int optval;
	pid_t pid;
	socklen_t cli_len;
	struct sockaddr_in cli_addr, serv_addr;
	char recv_buf[128];
	MASG masg; //声明结构体
			   /*数据库 初始化 链接****/
	MYSQL mysql;
	//初始化MYSQL句柄
	if (NULL == mysql_init(&mysql))
	{
		printf("init: %s\n", mysql_error(&mysql));
	}

	//建立MYSQL连接

	if (NULL == mysql_real_connect(&mysql, "localhost", "debian-sys-maint", "ZCoY8eh3ichrX4X0", "client_system", 0, NULL, 0))
	{
		printf("connect: %s\n", mysql_error(&mysql));
	}
	/******************/

	//创建一个tcp套接字
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	//设置该套接字让它可以重新绑定端口
	optval = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));

	//初始化服务器端地址结构
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//将套接字绑定到本地端口
	if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		my_err("bind", __LINE__);
	}

	//将套接字转化为监听套接字
	//  if(
	listen(sock_fd, LISTENQ);
	//<0);
	//  {
	//    my_err("listen",__LINE__);
	//  }
	cli_len = sizeof(struct sockaddr_in);
	printf("epoll初始化........\n");
	//printf("服务器sock_fd = %d\n",sock_fd);
	struct epoll_event ev, events[100];
	int epfd = epoll_create(100);
	setnonblocking(sock_fd); //设置非阻塞
	ev.data.fd = sock_fd;
	ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev);

	int socket_c = 0;
	int count = 0;
	while (1)
	{
		printf("等待用户操作.......\n");
		int nfds = epoll_wait(epfd, events, 100, -1);
		printf("操作中........\n");
		if (nfds == -1)
		{
			printf("epoll_wait failed\n");
			break;
		}
		int i;
		for (i = 0; i < nfds; i++)
		{
			//printf("nfds=%d\n",nfds);
			if (events[i].data.fd < 0)
				continue;
			if (events[i].data.fd == sock_fd)
			{
				socket_c = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
				printf("*********用户已链接,ip:%s\n", inet_ntoa(cli_addr.sin_addr));
				printf("当前用户套接字socket = %d\n", socket_c);
				if (socket_c > 0)
				{
					setnonblocking(sock_fd);
					ev.data.fd = socket_c;
					ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
					epoll_ctl(epfd, EPOLL_CTL_ADD, socket_c, &ev);
					continue;
				}
			}

			if (events[i].events & EPOLLERR) //?????
			{
				socket_c = events[i].data.fd;
				close(socket_c);
				epoll_ctl(epfd, EPOLL_CTL_DEL, socket_c, NULL);
				continue;
			}
			if (events[i].events & EPOLLHUP) //????
			{
				socket_c = events[i].data.fd;
				close(socket_c);
				epoll_ctl(epfd, EPOLL_CTL_DEL, socket_c, NULL);
				continue;
			}

			if (events[i].events & EPOLLIN) //遇到事件！！！
			{
				socket_c = events[i].data.fd;
				if (socket_c <= 0)
				{
					close(socket_c);
					epoll_ctl(epfd, EPOLL_CTL_DEL, socket_c, NULL);
					continue;
				}
				//char  recv_buf[1024];
				if (recv(socket_c, &masg, sizeof(MASG), 0) < 0)
				{
					perror("recv");
					exit(1);
				}

				printf("当前操作socket_c = %d\n", socket_c);
				//解包   查找选项

				int flag = masg.flag; //获取消息类型
				printf("获取消息类型为:%d\n", flag);

				switch (flag)
				{
				case CHAT_O:
					printf("........私聊操作...");
					chat_only(&mysql, &masg, socket_c);
					/*					if (socket_c == sock[0])
						send(sock[1], masg.data, sizeof(masg.data), 0);
					if (socket_c == sock[1])
						send(sock[0], masg.data, sizeof(masg.data), 0);
					printf("客户端发送消息：%s\n", masg.data);

					memset(&masg, 0, sizeof(MASG));
					printf("用户操作结束.........[返回等待]\n\n");
					*/
					break;

				case REGISTER:

					printf(".........注册.........\n");
					//执行插入语句
					char *sql = NULL;
					sql = (char *)calloc(1024, sizeof(char));
					sprintf(sql, "INSERT INTO client_member(name,passwd) VALUES('%s','%s')", masg.send_mem, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}

					masg.flag = REG_SUC;
					send(socket_c, &masg, sizeof(MASG), 0);
					//执行查询语句
					sql = "SELECT id,name,passwd FROM client_member";
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					MYSQL_RES *res = mysql_store_result(&mysql);
					MYSQL_ROW row;
					while ((row = mysql_fetch_row(res)))
					{
						printf("%s %s %s\n", row[0], row[1], row[2]);
					}
					//memset(sql,0,sizeof(sql));
					//mysql_free_result(res);
					//mysql_close(&mysql);
					break;

				case LOGIN: //登录

					sprintf(sql, "SELECT * FROM client_member where name='%s' and passwd='%s'", masg.send_mem, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					res = mysql_store_result(&mysql);

					if ((row = mysql_fetch_row(res)))
					{

						printf("%s 登陆成功\n", masg.send_mem);
						printf("%s %s %s\n", row[0], row[1], row[2]);
						///////注册进在线列表
						sprintf(sql, "INSERT INTO client_online (socket,name) VALUES('%d','%s')", socket_c, masg.send_mem);
						if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
						{
							printf("query: %s\n", mysql_error(&mysql));
						}
						printf("%s 加入在线列表\n", masg.send_mem);
						//////////////////

						masg.flag = LOG_SUC;
						send(socket_c, &masg, sizeof(MASG), 0);
					}
					else if ((!mysql_fetch_row(res)))
					{
						printf("登录失败！\n");
						masg.flag = LOG_FAU;
						send(socket_c, &masg, sizeof(MASG), 0);
					}
					break;

				case QUIT:

					printf("%s 已下线！\n", masg.send_mem);
					//从在线表中删除
					sprintf(sql, "DELETE FROM client_online WHERE name ='%s'", masg.send_mem);
					printf("从在线用户表中删除..\n");
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					masg.flag = QUIT;
					send(socket_c, &masg, sizeof(MASG), 0);
					break;

				case ADD_FRI:

					add_fri(&mysql, &masg, socket_c);
					break;

				case ADD_FRI_SCU: //添加好友成功
					//简化版
					sprintf(sql, "SELECT * FROM client_online where name='%s'", masg.send_mem);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					res = mysql_store_result(&mysql);

					if ((row = mysql_fetch_row(res)))
					{
						printf("%s 同意了 %s 的好友请求!\n", masg.recv_mem, masg.send_mem);

						printf("%s %s %s\n", row[0], row[1], row[2]);
						int socket_send = (int)row[1][0] - '0';
						masg.flag = ADD_FRI_SCU;
						printf("%d\n", socket_send);
						send(socket_send, &masg, sizeof(MASG), 0);
					}
					//数据库添加
					printf("正在加入好友列表............\n");
					sprintf(sql, "INSERT INTO client_friend (name_a,name_b) VALUES('%s','%s')", masg.send_mem, masg.recv_mem);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					sprintf(sql, "INSERT INTO client_friend (name_a,name_b) VALUES('%s','%s')", masg.recv_mem, masg.send_mem);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql)); //正反添加
					}

					break;

				case ADD_FRI_FAU: //添加好友失败
					//向发送方发送通知
					sprintf(sql, "SELECT * FROM client_online where name='%s'", masg.send_mem);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					res = mysql_store_result(&mysql);

					if ((row = mysql_fetch_row(res)))
					{
						printf("%s 拒绝了 %s 的好友请求!\n", masg.recv_mem, masg.send_mem);

						printf("%s %s %s\n", row[0], row[1], row[2]);
						int socket_send = (int)row[1][0] - '0';
						masg.flag = ADD_FRI_FAU;
						printf("%d\n", socket_send);
						send(socket_send, &masg, sizeof(MASG), 0);
					}
					break;

				case DEL_FRI:
					printf("%s 请求删除 %s\n", masg.send_mem, masg.recv_mem);
					printf("从好友表中删除..\n");
					sprintf(sql, "DELETE FROM client_friend WHERE name_a ='%s' and name_b = '%s'", masg.send_mem, masg.recv_mem);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					sprintf(sql, "DELETE FROM client_friend WHERE name_a ='%s' and name_b = '%s'", masg.recv_mem, masg.send_mem);

					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					printf("已删除....\n");
					int socket_send = select_c(masg.recv_mem, &mysql);
					masg.flag = DEL_FRI_MASG;
					send(socket_send, &masg, sizeof(MASG), 0);
					masg.flag = DEL_FRI_SCU;
					send(socket_c, &masg, sizeof(MASG), 0);

					break;

				case DIS_FRI:
					dis_fri_list(&mysql, &masg, socket_c);
					break; 
				case OUT_ROOM:
					socket_send = select_c(masg.recv_mem, &mysql);
					send(socket_send, &masg, sizeof(MASG), 0);
					break;
				case A_MESS:
					//保存为普通记录
					sprintf(sql, "INSERT INTO message_history (send,recv,message,status) VALUES('%s','%s','%s',0)", masg.send_mem, masg.recv_mem, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					printf("消息已存入聊天记录\n");
					break;
				case OFF_MESS:
					//保存为未读消息
					sprintf(sql, "INSERT INTO message_history (send,recv,message,status) VALUES('%s','%s','%s',1)", masg.send_mem, masg.recv_mem, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					printf("消息已加入未读列表!\n");
					break;

				case DIS_MES_ON: //查看未读消息
					dis_mes_on(&mysql,&masg,socket_c);
                    break;
				case DIS_MES_OFF://读取未读消息
					dis_mes_on(&mysql,&masg,socket_c);
					break;
				case DIS_MES_HIS://读取历史记录
					dis_mes_his(&mysql,&masg,socket_c);
					break;
				case CRE_GRO:
				    create_group(&mysql,&masg,socket_c);
					break;   
				case DIS_GRO:
				    dis_group(&mysql,&masg,socket_c);
					break;
				case INV_MEM:
				    invite_to_group(&mysql,&masg,socket_c);
					break;
				case CHAT_GRO:
					chat_group(&mysql,&masg,socket_c);
					break;
                case A_GRO_MESS:
					//保存为普通记录
					sprintf(sql, "INSERT INTO group_message (send,recv,qun,message,status) VALUES('%s','%s','%s','%s',0)", masg.send_mem, masg.recv_mem,masg.group, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					printf("消息已存入群聊天记录\n");
					break;
				case OFF_GRO_MESS:
					//保存为未读消息
					sprintf(sql, "INSERT INTO group_message (send,recv,qun,message,status) VALUES('%s','%s','%s','%s',1)", masg.send_mem, masg.recv_mem,masg.group, masg.data);
					if (0 != mysql_real_query(&mysql, sql, strlen(sql)))
					{
						printf("query: %s\n", mysql_error(&mysql));
					}
					printf("消息已加入群未读列表!\n");
					break;
				case DIS_GRO_MEM://显示群成员
					dis_group_member(&mysql,&masg,socket_c);
					break;
				   
				} //switch }
			}	 //事件 }
			//  printf("轮寻.......\n");
		} //for }
		  //  printf("while........\n");
	}	 //while }
		  //  printf("服务器关闭.......\n");

	// mysql_free_result(res);
	// mysql_close(&mysql);    //关闭数据库
}
