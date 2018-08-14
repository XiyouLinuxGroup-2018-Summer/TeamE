#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
    printf("%s structure at 0x%lx\n", s, (unsigned long)fp);
    printf(" foo.a = %d\n foo.b = %d\n foo.c = %d\n foo.d = %d\n", 
            fp->a, fp->b, fp->c, fp->d);
}

void *thr_fn1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};

    printfoo("thread 1:\n", &foo);
    pthread_exit((void *)&foo);
}

void *thr_fn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
    pthread_exit((void *)0);
}

int main(void)
{
    int         err;
    pthread_t   tid1, tid2;
    struct foo  *fp;

    if ((err = pthread_create(&tid1, NULL, thr_fn1, NULL)) != 0)
        err_exit(err, "can't creat thread 1");
    if ((err = pthread_join(tid1, (void *)&fp)) != 0)
        err_exit(err, "can't join with thread 1");
    sleep(1);
    printf("parent starting second thread\n");
    if ((err = pthread_create(&tid2, NULL, thr_fn2, NULL)) != 0)
        err_exit(err, "can't creat thread 2");
    sleep(1);
    printfoo("parent:\n", fp);
    exit(0);
}
