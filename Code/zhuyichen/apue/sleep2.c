#include "apue.h"
#include <setjmp.h>

static jmp_buf env_alarm;

static void sig_alrm(int signo)
{
    longjmp(env_alarm, 1);
}

unsigned sleep2(unsigned seconds)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return seconds;
    if (setjmp(env_alarm) == 0) {
        alarm(seconds);
        pause();
    }
    return alarm(0);
}
