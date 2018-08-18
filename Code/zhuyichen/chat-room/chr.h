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
#include </usr/local/mysql/include/mysql.h>

#define LQMAX       32
#define SERVPORT    4507
#define MAXEVENTS   1024
#define MAXNAME     32
#define MAXPASSWD   32

#define max(x, y)   ((x) > (y) ? (x) : (y))

extern struct kevent events[];

MYSQL *initmysql(void);
void check_login(int connfd, int i);
void Mysql_query(MYSQL *con, const char *str);

ssize_t Readn(int fd, void *vptr, size_t nsize);
ssize_t Writen(int fd, const void *vptr, size_t n);
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);
void err_msg(const char *fmt, ...);

#endif
