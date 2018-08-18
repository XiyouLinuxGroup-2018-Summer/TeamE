#include "chr.h"

void err_sys(const char *fmt, ...)
{
    va_list ap;
    char    buf[BUFSIZ];

    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ - 1, fmt, ap); 
    snprintf(buf + strlen(buf), BUFSIZ - strlen(buf) - 1, 
            ": %s\n", strerror(errno));
    fputs(buf, stderr);
    va_end(ap);
    exit(1);
}

void err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

void err_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}
