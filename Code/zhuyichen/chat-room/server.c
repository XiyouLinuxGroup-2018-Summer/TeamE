#include "chr.h"

struct kevent events[MAXEVENTS];

int main(void)
{
    int                 listenfd, sockfd, connfd;
    socklen_t           clilen;
    struct sockaddr_in  servaddr, cliaddr;
    int                 n, nev, kq, i;
    struct kevent       kev[1];
    static char         buf[BUFSIZ];

    /*  创建一个套接字  */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    /*  初始化服务器地址结构  */
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*  将套接字绑定到本地端口  */
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("bind error");

    /*  将套接字转换为监听套接字  */
    if (listen(listenfd, LQMAX) < 0)
        err_sys("listen error");

    kq = kqueue();
    EV_SET(&kev[0], listenfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, kev, 1, NULL, 0, NULL) < 0)
        err_sys("kevent error");

    while (1) {
        if ((nev = kevent(kq, NULL, 0, events, MAXEVENTS, NULL)) < 0)
            err_sys("kevent error");

        for (i = 0; i < nev; i++)
            sockfd = events[i].ident;
            if (sockfd == listenfd) {
                if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                    err_sys("accept error");

                EV_SET(&kev[0], connfd, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                if (kevent(kq, kev, 1, NULL, 0, NULL) < 0)
                    err_sys("kevent error");

                check_login(connfd, i);    /*  检查登陆  */
            } else {
                if ((n = Readn(sockfd, buf, sizeof(buf))) == 0) {    /*  用户下线  */
                    events[i].flags = EV_DELETE;
                    close(sockfd);
                } else
                    Writen(sockfd, buf, n);
            }
    }
    exit(0);
}
