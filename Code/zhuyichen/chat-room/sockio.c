#include "chr.h"

/*  
 * 这两个函数用于网络传输数据，必须配套使用
 * 先使用writen发送指定字节的数据，再使用readn读指定字节的数据，顺序不能颠倒
 */

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
    static char buf[MAXIDENT];

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

/*  readen函数：从一个字符流套接字中读n字节  */
ssize_t readen(int fd, void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nread;
    char        *ptr;

    ptr = vptr;
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

ssize_t Readn(int fd, void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = readn(fd, vptr, nsize)) < 0)
        err_sys("read error");
    return n;
}

ssize_t Writen(int fd, const void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = writen(fd, vptr, nsize)) < 0)
        err_sys("write error");
    return n;
}

ssize_t Readen(int fd, void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = readen(fd, vptr, nsize)) < 0)
        err_sys("read error");
    return n;
}

void sendf(char *buf, int sockfd)
{
    int     fd, n;

    if ((fd = open(buf + 3, O_RDONLY)) < 0)
        err_sys("open error");
    while ((n = Readen(fd, buf, BUFSIZ)) > 0)
        Writen(sockfd, buf, n);
    printf("Delivered\n");
    close(fd);
}

void recvf(char *buf, int sockfd)
{
    int fd, n;

    if ((fd = open("./tmp", O_WRONLY | O_APPEND | O_CREAT, 0755)) < 0)
        err_sys("open error");
    while ((n = Readn(sockfd, buf, BUFSIZ)) > 0)
        Writen(fd, buf, n);
    printf("Received\n");
    close(fd);
}
