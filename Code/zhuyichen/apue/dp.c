#include "apue.h"
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int     fd[2];
    pid_t   pid;
    char    *pager, *argv0;
    char    line[MAXLINE];
    FILE    *fp;

    if (argc != 2)
        err_sys("usage: a.out [pathname]");

    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);
    if (pipe(fd) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid > 0) {
        close(fd[0]);
        while (fgets(line, MAXLINE, fp) != NULL)
            if (write(fd[1], line, strlen(line) != strlen(line)))
                err_sys("write error to pipe");
    } else {
        close(fd[1]);
        if (fd[0] != 0)
            if (dup2(fd[0], 0) != 0)
                err_sys("dup2 error to stdin");
    }

    if ((pager = getenv("PAGER")) == NULL)
        pager = DEF_PAGER;
    if ((argv0 = strrchr(pager, '/')) != NULL)
        argv0++;
    else
        argv0 = pager;

    if (execl(pager, argv0, (char *)0) < 0)
        err_sys("execl error");
    exit(0);
}
