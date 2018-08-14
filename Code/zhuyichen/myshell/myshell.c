#include "myshell.h"

static char whitespace[] = " \t\r\n\v";
static char symbols[] = "<|>&;()";
static char prompt[MAXLINE];    /*  保存shell提示字符  */
static int  flag;    /*  标记是否执行过cd [dir]  */

int main(void)
{
    char buf[MAXLINE];

    signal(SIGINT, signal_handler);

    while (1) {
        get_prompt();
        getcmd(buf, sizeof(buf));
        if (builtin_cmd(buf))
            continue;
        if (Fork() == 0)
            runcmd(parsecmd(buf));
        wait(0);
    }
}

/*  signal_handler函数：信号处理函数  */
void signal_handler(int signo)
{
    ;    /*  忽略Ctr + C  */
}

/*  getcmd函数：读取一行输入  */
void getcmd(char *buf, int nbuf)
{
    char *tmp;
    
    memset(buf, 0, nbuf);
    if ((tmp = readline(prompt)) == NULL) {    /*  tab自动补全  */
        fprintf(stderr, "\n");    /*  键入了EOF, 则退出shell  */
        exit(0);
    }
    add_history(tmp);    /*  可上下翻阅历史命令  */
    strcpy(buf, tmp);
    free(tmp);
}

/*  builtin_cmd函数：处理shell的一些内置命令  */
int builtin_cmd(char *buf)
{
    char *tf;

    while (*buf && isspace(*buf))
        buf++;
    if (*buf == '\0')
        return 0;
    if (strncmp("exit", buf, 4) == 0)
        exit(0);
    else if (strncmp("cd", buf, 2) == 0) {
        for (buf += 2; *buf && isspace(*buf); buf++)
            ;
        tf = buf;
        while (*buf && !isspace(*buf))
            buf++;
        *buf = '\0';
        if (tf < buf)
            get_pwd(tf);
        return flag = (tf < buf) ? 1 : 0;
    }
    return 0;
}

/*  replace函数：将HOME路径替换为~  */
char *replace(char *s)
{
    char *ps, *es, *ts;

    ts = s;
    ps = s + strlen(getenv("HOME"));
    es = s + strlen(s); 
    *s++ = '~';
    while (ps < es)
        *s++ = *ps++;
    *s = '\0';
    return ts;
}

/*  expand函数：将~扩充为HOME路径  */
char *expand(char *s)
{
    char *ts;

    ts = Malloc(sizeof(s + 1));   /*  不需要'~'字符  */
    strcpy(ts, s + 1);
    strcpy(s, getenv("HOME"));
    strcat(s, ts);
    return s;
}

/*  get_prompt函数：获得提示符  */
void get_prompt(void)
{
    struct passwd   *usr;

    if ((usr = getpwuid(getuid())) == NULL)
        error("getpwuid");

    if (!flag) {    /*  默认情况  */
        if (chdir(getenv("HOME")) < 0)
            error("chdir");
        sprintf(prompt, GREEN"[[myshell]"END CYAN"@"END RED"%s]:"END YELLOW"~"END
                " "RED"$"END" ", usr->pw_name);
    }
}

/*  get_pwd函数：改变当前工作目录  */
void get_pwd(char *s)
{
    struct passwd   *usr;
    char            path[MAXLINE];
    char            *p;

    if ((usr = getpwuid(getuid())) == NULL)
        error("getpwuid");

    if (s[0] == '~')
        s = expand(s);
    if (chdir(s) < 0)
        error("chdir");
    if ((p = getcwd(path, MAXLINE)) == NULL)
        error("getcwd");
    sprintf(prompt, GREEN"[[myshell]"END CYAN"@"END RED"%s]:"END YELLOW"%s"END
                " "RED"$"END" ", usr->pw_name, !strncmp(getenv("HOME"), path, sizeof(p)) ? 
                replace(path) : path);
}

/*  parsecmd函数：解析输入行，构造出链式命令串  */
struct cmd *parsecmd(char *s)
{
    char *es;
    struct cmd *cmd;

    es = s + strlen(s);   
    cmd = parseline(&s, es);
    peek(&s, es, "");
    if (s != es)
        err_exit("syntax - missing (");
    nulterminate(cmd);
    return cmd;
}

/*  
 * 下面几个函数通过一连串的相互递归调用，
 * 构建起链式的可执行命令串
 */

/*******************************************************************/

struct cmd *parseline(char **ps, char *es)
{
    struct cmd *cmd;
  
    cmd = parsepipe(ps, es);    /* 以管道为单位划分命令串 */
    while (peek(ps, es, "&")) {    /* 有后台运行符 */
        gettoken(ps, es, 0, 0);
        cmd = backcmd(cmd);
    }
    if (peek(ps, es, ";")) {    /* 有并列命令 */
        gettoken(ps, es, 0, 0);
        cmd = listcmd(cmd, parseline(ps, es));    /* 递归调用自身来构造并列命令 */
    }
    return cmd;
}

struct cmd *parsepipe(char **ps, char *es)
{
    struct cmd *cmd;

    cmd = parseexec(ps, es);
    if (peek(ps, es, "|")) {
        gettoken(ps, es, 0, 0);
        cmd = pipecmd(cmd, parsepipe(ps, es));    /* 递归调用自身以构造管道命令 */
    }
    return cmd;
}

struct cmd *parseexec(char **ps, char *es)
{
    char            *q, *eq;   /* 分别指向exec命令的起始和末尾 */
    int             tok, argc;
    struct execcmd  *ecmd;
    struct cmd      *cmd;

    if (peek(ps, es, "("))
        return parseblock(ps, es);

    cmd = execcmd();
    ecmd = (struct execcmd *)cmd;
    argc = 0;
    cmd = parseredirs(cmd, ps, es);

    while (!peek(ps, es, "|)&;")) {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a')
            err_exit("syntax");
        ecmd->argv[argc] = q;
        ecmd->eargv[argc] = eq;
        if (++argc >= MAXARGS)
            err_exit("too many args");
        cmd = parseredirs(cmd, ps, es);   /* 构造完基本命令后，可能还存在着重定向命令 */
    }
    ecmd->argv[argc] = 0;
    ecmd->eargv[argc] = 0;
    return cmd;
}

struct cmd *parseblock(char **ps, char *es)
{
    struct cmd *cmd;

    if (!peek(ps, es, "("))
        err_exit("parseblock");
    gettoken(ps, es, 0, 0);
    cmd = parseline(ps, es);
    if (!peek(ps, es, ")"))
        err_exit("syntax - missing )");
    gettoken(ps, es, 0, 0);
    cmd = parseredirs(cmd, ps, es);
    return cmd;
}

struct cmd *parseredirs(struct cmd *cmd, char **ps, char *es)
{
    int tok;
    char *q, *eq;

    while (peek(ps, es, "<>")){
        tok = gettoken(ps, es, 0, 0);
        if (gettoken(ps, es, &q, &eq) != 'a')
            err_exit("syntax - missing file for redirection");
        switch (tok) {
        case '<':
            cmd = redircmd(cmd, q, eq, O_RDONLY, 0,  0);
            break;
        case '>':
            cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 0755, 1);
            break;
        case '+':   /* '>>' */
            cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 0755, 1);
            break;
        }
    }
    return cmd;
}
/*******************************************************************/

/*  
 *  gettoken函数：词法提取函数
 *  忽略symbols中的字符，提取出exec命令名或重定向文件名
 */
int gettoken(char **ps, char *es, char **q, char **eq)
{
    char    *s;
    int     ret;
  
    s = *ps;
    while (s < es && strchr(whitespace, *s))
        s++;     /*  跳过空白符  */
    if (q != NULL)
        *q = s;
    ret = *s;

    switch (*s) {
    case 0:
        break;
    case '|': case '(': case ')':
    case ';': case '&': case '<':    /*  简单的跳过  */
        s++;
        break;
    case '>':
        if (*++s == '>') {   /*  '>>'  */
            ret = '+';    /*  遇见了一个>>  */
            s++;
        }
        break;
    default:
        ret = 'a';    /*  遇见了一个exec命令  */
        while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
          s++;
        break;
    }
    if (eq != NULL)
        *eq = s;
    while (s < es && strchr(whitespace, *s))
        s++;
    *ps = s;
    return ret;
}

/*  
 * peek函数：将*ps设置为下一个非空白符的位置，
 * 如果该位置的字符出现在toks中，则返回真  
 */ 
int peek(char **ps, char *es, char *toks)
{
    char *s;

    s = *ps;
    while (s < es && strchr(whitespace, *s)) 
        s++;
    *ps = s;
    return *s && strchr(toks, *s);
}

/*   nulterminate函数：给每个构造完成的命令串末尾加上'\0'  */
struct cmd *nulterminate(struct cmd *cmd)
{
    struct backcmd    *bcmd;
    struct execcmd    *ecmd;
    struct listcmd    *lcmd;
    struct pipecmd    *pcmd;
    struct redircmd   *rcmd;

    if (cmd == NULL)
        return NULL;

    switch (cmd->type) {
    case EXEC:
        ecmd = (struct execcmd *)cmd;
        for(int i = 0; ecmd->argv[i]; i++)
            *ecmd->eargv[i] = 0;
        break;
    case REDIR:
        rcmd = (struct redircmd *)cmd;
        nulterminate(rcmd->cmd);
        *rcmd->efile = 0;
        break;
    case PIPE:
        pcmd = (struct pipecmd *)cmd;
        nulterminate(pcmd->left);
        nulterminate(pcmd->right);
        break;
    case LIST:
        lcmd = (struct listcmd *)cmd;
        nulterminate(lcmd->left);
        nulterminate(lcmd->right);
        break;
    case BACK:
        bcmd = (struct backcmd *)cmd;
        nulterminate(bcmd->cmd);
        break;
    }
    return cmd;
}

/*  
 * 下面5个函数，为对应的结构体分配内存并填充其结构体
 */

/*******************************************************************/
struct cmd *execcmd(void)
{
    struct execcmd *cmd;

    cmd = Malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;
    return (struct cmd *)cmd;
}

struct cmd *redircmd(struct cmd *subcmd, char *file, char *efile, int flag, int mode, int fd)
{
    struct redircmd *cmd;

    cmd = Malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->efile = efile;
    cmd->flag = flag;
    cmd->mode = mode;
    cmd->fd = fd;
    return (struct cmd *)cmd;
}

struct cmd *pipecmd(struct cmd *left, struct cmd *right)
{
    struct pipecmd *cmd;

    cmd = Malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    return (struct cmd *)cmd;
}

struct cmd *listcmd(struct cmd *left, struct cmd *right)
{
    struct listcmd *cmd;

    cmd = Malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = LIST;
    cmd->left = left;
    cmd->right = right;
    return (struct cmd *)cmd;
}

struct cmd *backcmd(struct cmd *subcmd)
{
    struct backcmd *cmd;

    cmd = Malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = BACK;
    cmd->cmd = subcmd;
    return (struct cmd *)cmd;
}
/*******************************************************************/

/*  runcmd函数：执行构造好的链式命令串  */
void runcmd(struct cmd *cmd)
{
    struct backcmd  *bcmd;
    struct execcmd  *ecmd;
    struct listcmd  *lcmd;
    struct pipecmd  *pcmd;
    struct redircmd *rcmd;
    int             fd[2];

    if (cmd == 0)
        exit(0);
    switch (cmd->type) {
    case EXEC:
        ecmd = (struct execcmd *)cmd;
        if (ecmd->argv[0] == 0)
            exit(0);
        execvp(ecmd->argv[0], ecmd->argv);
        err_exit("%s: command not found", ecmd->argv[0]);
        break;
    case REDIR:
        rcmd = (struct redircmd *)cmd;
        close(rcmd->fd);
        if (open(rcmd->file, rcmd->flag, rcmd->mode) < 0)
            err_exit("can't open %s", rcmd->file);
        runcmd(rcmd->cmd);
        break;
    case LIST:
        lcmd = (struct listcmd *)cmd;
        if (Fork() == 0)
            runcmd(lcmd->left);
        wait(0);
        runcmd(lcmd->right);
        break;
    case PIPE:
        pcmd = (struct pipecmd *)cmd;
        if (pipe(fd) < 0)
            err_exit("pipe");
        if (Fork() == 0) { 
            dup2(fd[1], 1);   /*  将标准输出重定向到管道写端  */
            close(fd[0]);
            close(fd[1]);
            runcmd(pcmd->left);
        }
        if (Fork() == 0) {   
            dup2(fd[0], 0);   /*  将标准输入重定向到管道读端  */
            close(fd[0]);
            close(fd[1]);
            runcmd(pcmd->right);
        }
        close(fd[0]);
        close(fd[1]);
        wait(0);
        wait(0);
        break;
    case BACK:
        bcmd = (struct backcmd *)cmd;
        if (Fork() == 0)
            runcmd(bcmd->cmd);
        break;
    default:
        err_exit("runcmd");
    }
    exit(0);
}

/*  Fork函数：fork的包装函数  */
int Fork(void)
{
    int pid;

    if ((pid = fork()) < 0)
        err_exit("fork");
    return pid;
}

/*  Malloc函数：malloc的包装函数  */
void *Malloc(size_t nbytes)
{
    void *p;

    if ((p = malloc(nbytes)) == NULL)
        err_exit("alloc failed");
    return p;
}

/*  error函数：打印出错信息，并终止程序运行  */
void err_exit(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, RED"error:"END" ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

/*  error函数：打印出错信息  */
void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, RED"error:"END" ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
