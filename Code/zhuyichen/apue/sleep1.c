#include "apue.h"

void sig_alarm(int signo)
{
    ;
}

unsigned sleep1(unsigned seconds)
{
    if (signal(SIGALRM, sig_alarm) == SIG_ERR)
        return seconds;
    alarm(seconds);
    pause();
    return alarm(0);
}
