#include "chr.h"

/*  check_login函数：处理用户登陆或注册  */
void check_login(int connfd, int i)
{
    static char query[BUFSIZ]; 
    static char name[MAXNAME];
    static char passwd[MAXPASSWD];
    MYSQL       *con;
    MYSQL_RES   *res;
    int         n;

    con = initmysql();

    Writen(connfd, "1.login\n2.register\n", 19);

last:
    if ((n = Readn(connfd, query, sizeof(query))) == 0) {
        events[i].flags = EV_DELETE;
        close(connfd);
    }
    if (strcmp(query, "1\n") == 0) {    /*  登陆  */
login_nrep:
        Writen(connfd, "username:", 9);
        if ((n = Readn(connfd, name, sizeof(name))) == 0) {
            events[i].flags = EV_DELETE;
            close(connfd);
        }
        if (name[n - 1] == '\n')
            name[n - 1] = '\0';

        sprintf(query, "select * from userinfo where name='%s';", name);
        Mysql_query(con, query);
        if ((res = mysql_store_result(con)) == NULL)
            err_sys("mysql_store_result error");
        if (!mysql_num_rows(res)) {
            Writen(connfd, "username dosen't exist\n", 24);
            goto login_nrep;
        }
        mysql_free_result(res);

login_prep:
        Writen(connfd, "password:", 9);
        if ((n = Readn(connfd, passwd, sizeof(passwd))) == 0) {
            events[i].flags = EV_DELETE;
            close(connfd);
        }
        if (passwd[n - 1] == '\n')
            passwd[n - 1] = '\0';

        sprintf(query, "select * from userinfo where passwd='%s';", passwd);
        Mysql_query(con, query);
        if ((res = mysql_store_result(con)) == NULL)
            err_sys("mysql_store_result error");
        if (!mysql_num_rows(res)) {
            Writen(connfd, "wrong password\n", 15);
            goto login_prep;
        }
        mysql_free_result(res);

    } else if (strcmp(query, "2\n") == 0) {    /*  注册  */
regis_rep:
        Writen(connfd, "username:", 9);
        if ((n = Readn(connfd, name, sizeof(name))) == 0) {
            events[i].flags = EV_DELETE;
            close(connfd);
        }
        if (name[n - 1] == '\n')
            name[n - 1] = '\0';

        sprintf(query, "select * from userinfo where name='%s';", name);
        Mysql_query(con, query);
        if ((res = mysql_store_result(con)) == NULL)
            err_sys("mysql_store_result error");
        if (mysql_num_rows(res)) {
            Writen(connfd, "username already exists\n", 24);
            goto regis_rep;
        }
        mysql_free_result(res);

        Writen(connfd, "password:", 9);
        if ((n = Readn(connfd, passwd, sizeof(passwd))) == 0) {
            events[i].flags = EV_DELETE;
            close(connfd);
        }
        if (passwd[n - 1] == '\n')
            passwd[n - 1] = '\0';

        sprintf(query, "insert into userinfo (name, passwd, socket) values('%s', '%s', %d);", 
            name, passwd, connfd);
        Mysql_query(con, query);
    } else
        goto last;

    mysql_close(con);
}
