#ifndef _SH_H
#define _SH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <readline/readline.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>

#define MAXLINE     4096
#define MAXARGS     100

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define END     "\033[0m"

#define EXEC    1
#define REDIR   2
#define PIPE    3
#define LIST    4
#define BACK    5


struct cmd {
    int type;
};

struct execcmd {     /*  普通可执行命令及文件  */
    int     type;
    char    *argv[MAXARGS];    /*  指向每个参数串的开始  */
    char    *eargv[MAXARGS];   /*  指向每个参数串的末尾  */
};

struct redircmd {    /*  重定向命令  */
    int         type;
    struct cmd  *cmd;
    char        *file;      /*  指向文件名的起始  */
    char        *efile;     /*  指向文件名的末尾  */
    int         flag;       /*  文件访问模式  */
    int         mode;       /*  创建文件时的权限  */
    int         fd;
};

struct pipecmd {    /*  管道命令  */
    int         type;
    struct cmd  *left;      /*  左命令  */
    struct cmd  *right;     /*  右命令  */
};

struct listcmd {    /*  并列命令  */
    int         type;
    struct cmd  *left;
    struct cmd  *right;
};

struct backcmd {    /*  后台运行命令  */
    int         type;
    struct cmd  *cmd;
};

int builtin_cmd(char *buf);
void get_prompt(void);
void get_pwd(char *s);

void signal_handler(int signo);
void getcmd(char *buf, int nbuf);
void runcmd(struct cmd *cmd);
int gettoken(char **ps, char *es, char **q, char **eq);
int peek(char **ps, char *es, char *toks);

struct cmd *parsecmd(char *s);
struct cmd *parseline(char **ps, char *es);
struct cmd *parsepipe(char **ps, char *es);
struct cmd *parseexec(char **ps, char *es);
struct cmd *parseblock(char **ps, char *es);
struct cmd *parseredirs(struct cmd *cmd, char **ps, char *es);
struct cmd *nulterminate(struct cmd *cmd);

struct cmd *execcmd(void);
struct cmd *redircmd(struct cmd *subcmd, char *file, char *efile, int flag, int mode, int fd);
struct cmd *pipecmd(struct cmd *left, struct cmd *right);
struct cmd *listcmd(struct cmd *left, struct cmd *right);
struct cmd *backcmd(struct cmd *subcmd);

void err_exit(char *fmt, ...);
void error(char *fmt, ...);
int Fork(void);
void *Malloc(size_t nbytes);

#endif    /*  _SH_H  */
