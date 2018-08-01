#include "apue.h"

unsigned sleep2(unsigned);
void sig_int(int);

int main(void)
{
    unsigned unslept;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    unslept = sleep2(5);
    printf("sleep2 returned: %u\n", unslept);
    exit(0);
}

void sig_int(int signo)
{
    int i, j;
    volatile int k;

    printf("\nsig_int starting\n");
    k = 0;
    for (i = 0; i < 300000; i++)
        for (j = 0; j < 4000; j++)
            k += i * j;
    printf("sig_int finished\n");
}
