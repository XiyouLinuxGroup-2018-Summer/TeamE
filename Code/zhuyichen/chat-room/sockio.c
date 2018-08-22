#include "chr.h"

/*  readn函数：从一个字符流套接字中至多读nsize字节  */
ssize_t readn(int fd, void *vptr, size_t nsize)
{
    size_t      nleft, n;
    ssize_t     nread;
    char        *ptr;

    bzero(vptr, nsize);
    n = 0;
    ptr = vptr;
    while (1) {
        if ((nread = read(fd, ptr, 1)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            return 0;
        if (*ptr == '\r')
            break;
        else
            n = n * 10 + (*ptr - '0');
    }

    if (n > nsize)
        n = nsize;
    nleft =  n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;   /*  中断后重启read  */
            else
                return -1;
        } else if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

/*  writen函数：向一个字符流套接字中写nsize字节  */
ssize_t writen(int fd, const void *vptr, size_t nsize)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;
    static char buf[32];

    bzero(buf, sizeof(buf));
    sprintf(buf, "%zu\r", nsize);

    ptr = buf;
    nleft = strlen(buf);
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
        
    ptr = vptr;
    nleft = nsize;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return nsize;
}
