#ifndef _CHR_H
#define _CHR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netdb.h>
#include "/usr/local/mysql/include/mysql.h"

#define LQMAX       32
#define SERVPORT    4507
#define MAXEVENTS   1024
#define MAXNAME     32
#define MAXPASSWD   32

enum {
    LOGIN = 1,      /*  登陆  */
    REGISTER,       /*  注册  */
    CHPASSWD,       /*  修改密码  */
    ADDFRIEND,      /*  加好友  */
    DELFRIEND,      /*  删除好友  */
    PRVC,           /*  私聊  */
    GRPC,           /*  群聊  */
    VIEW,           /*  查看聊天记录  */
    SHIELD,         /*  屏蔽某人消息  */
    ADDGRP,         /*  加群  */
    CRTGRP,         /*  创建群  */
    LEAVEGRP,       /*  退群  */
    SETADMINIS,     /*  设置管理员  */
    KICK            /*  踢人  */
};

#define max(x, y)   ((x) > (y) ? (x) : (y))

extern struct kevent events[];
extern MYSQL *con;

MYSQL *initmysql(void);
void logout(int sockfd);
int get_serv_type(char *buf, int sockfd);
void process_msg(char *buf, int sockfd);
void check_login(char *buf, int sockfd);
void check_register(char *buf, int sockfd);
void add_friend(char *name, int sockfd, int serv_type);
void del_friend(char *name, int sockfd, int serv_type);
void private_chat(char *msg, int sockfd, int serv_type);

void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);
void err_msg(const char *fmt, ...);

/*  wrap functions  */
int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Kqueue(void);
int Kevent(int kq, const struct kevent *changelist, int nchanges,
        struct kevent *eventlist, int nevents, const struct timespec *timeout);
int Select(int maxfdpl, fd_set *rset, fd_set *wset, fd_set *eset, 
        struct timeval *timeout);
char *Fgets(char *ptr, int n, FILE *stream);
ssize_t Readn(int fd, void *vptr, size_t nsize);
ssize_t Writen(int fd, const void *vptr, size_t nsize);
void Mysql_query(MYSQL *con, const char *str);
MYSQL_RES *Mysql_store_result(MYSQL *con);

#endif
