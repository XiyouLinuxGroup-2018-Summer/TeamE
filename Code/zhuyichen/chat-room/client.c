#include "chr.h"


int main(void)
{
    int                 connfd, n;
    struct sockaddr_in  cliaddr;
    fd_set              fdset, tmpset;
    char                buf[BUFSIZ];

    cliaddr.sin_port = htons(SERVPORT);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connfd = Socket(AF_INET, SOCK_STREAM, 0);

    Connect(connfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    FD_ZERO(&fdset);
    FD_SET(0, &fdset);
    FD_SET(connfd, &fdset);

    while (1) {
        tmpset = fdset;

        /*  监听用户输入和服务端  */
        Select(connfd + 1, &tmpset, NULL, NULL, NULL);

        if (FD_ISSET(0, &tmpset)) {
            Fgets(buf, sizeof(buf), stdin);
            n = strlen(buf);
            if (buf[n - 1] == '\n')   /*  删除行末换行符  */
                buf[n - 1] = '\0';
            else
                n++;    /*  包含末尾'\0'字符  */
            if (*buf)
                Writen(connfd, buf, n);
        }

        if (FD_ISSET(connfd, &tmpset)) {
            if ((n = Readn(connfd, buf, sizeof(buf))) == 0) {    /*  服务端退出  */
                err_msg("server exit");                
                break;
            }
            if (strncmp(buf, "ys\r", 3) == 0)   /*  发送文件  */
                sendf(buf, connfd);
            else if (strncmp(buf, "yr\r", 3) == 0)   /*  接收文件  */
                recvf(buf, connfd);
            else
                Writen(1, buf, n);   /*  写到标准输出  */
        }
    }
    exit(0);
}
