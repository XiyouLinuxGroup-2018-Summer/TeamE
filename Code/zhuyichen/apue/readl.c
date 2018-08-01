#include "apue.h"

static void sig_alrm(int signo)
{
    ;
}

int main(void)
{
    int n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    alarm(10);
    if ((n = read(0, line, MAXLINE)) < 0)
        err_sys("read error");

    write(1, line, n);
    exit(0);
}
