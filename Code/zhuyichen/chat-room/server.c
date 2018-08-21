#include "chr.h"

MYSQL           *con;
struct kevent   events[MAXEVENTS];

int main(void)
{
    int                 listenfd, sockfd, connfd;
    socklen_t           clilen;
    struct sockaddr_in  servaddr, cliaddr;
    int                 nev, kq, i;
    struct kevent       kev[1];
    static char         buf[BUFSIZ];

    con = initmysql();

    /*  创建一个套接字  */
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    /*  初始化服务器地址结构  */
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*  将套接字绑定到本地端口  */
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /*  将套接字转换为监听套接字  */
    Listen(listenfd, LQMAX); 

    kq = Kqueue();
    EV_SET(&kev[0], listenfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    Kevent(kq, kev, 1, NULL, 0, NULL);

    while (1) {
        nev = Kevent(kq, NULL, 0, events, MAXEVENTS, NULL);

        for (i = 0; i < nev; i++)
            if (events[i].ident == listenfd) {   /*  处理用户连接请求  */
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

                /*  将connfd加入监听队列  */
                EV_SET(&kev[0], connfd, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                Kevent(kq, kev, 1, NULL, 0, NULL);

                help(connfd);
            } else {    /*  接受用户消息  */
                sockfd = events[i].ident;
                if (Readn(sockfd, buf, sizeof(buf)) == 0) {    /*  用户下线  */
                    logout(sockfd);
                    events[i].flags |= EV_DELETE;
                    close(sockfd);
                } else
                    process_msg(buf, sockfd);    /*  处理用户请求  */
            }
    }
    mysql_close(con);
    exit(0);
}

void process_msg(char *buf, int sockfd)
{
    int     serv_type;
    char    sql[BUFSIZ];

    /*  无服务状态  */
    if (!(serv_type = get_serv_type(buf, sockfd))) {    
        if (strcmp(buf, "\\h") == 0)
            help(sockfd);
        else if (strcmp(buf, "\\c") == 0)
            clean(sockfd);
    } else if (strcmp(buf, "\\q") == 0) {  /*  处于服务状态时，退出某个服务  */
        sprintf(sql, "update userinfo set serv_type=0, chat_id=0, chat_fd=0 "
            "where socket=%d;", sockfd);    
        Mysql_query(con, sql);
        return;
    }

    /*  处理服务请求  */
    switch (serv_type) {
    case LOGIN: 
        check_login(buf, sockfd); break;
    case REGISTER:
        check_register(buf, sockfd); break;
    case CHPASSWD: 
        change_passwd(buf, sockfd); break; 
    case ADDFRIEND: 
        add_friend(buf, sockfd, serv_type); break;
    case DELFRIEND:
        del_friend(buf, sockfd); break;
    case PRVC:
        private_chat(buf, sockfd, serv_type); break;
    case GRPC:
        break;
    case VIEW:
        break;
    case SHIELD:
        break;
    case ADDGRP: 
        add_group(buf, sockfd); break;
    case CRTGRP:
        creat_group(buf, sockfd); break;
    case LEAVEGRP:
        leave_group(buf, sockfd); break;
        break;
    case SETADMINIS:
        break;
    case KICK:
        break;
    default:
        break;
    }
}
