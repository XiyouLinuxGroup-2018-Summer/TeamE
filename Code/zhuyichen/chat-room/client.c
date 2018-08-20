#include "chr.h"

int main(int argc, char **argv)
{
    int                 connfd, n;
    struct sockaddr_in  cliaddr;
    fd_set              fdset, tmpset;
    char                buf[BUFSIZ];

    cliaddr.sin_port = htons(SERVPORT);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    if (connect(connfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
        err_sys("connect error");

    FD_ZERO(&fdset);
    FD_SET(0, &fdset);
    FD_SET(connfd, &fdset);

    while (1) {
        tmpset = fdset;

        if (select(connfd + 1, &tmpset, NULL, NULL, NULL) < 0)
            err_sys("select error");

        if (FD_ISSET(0, &tmpset)) {
            bzero(buf, sizeof(buf));
            if (fgets(buf, sizeof(buf), stdin) == NULL && ferror(stdin))
                err_sys("fgets error");
            
            Writen(connfd, buf, strlen(buf));
        }

        if (FD_ISSET(connfd, &tmpset)) {
            if ((n = Readn(connfd, buf, sizeof(buf))) == 0) {
                err_msg("server exit");                
                break;
            } else
                Writen(1, buf, n);
        }
    }
    exit(0);
}
