#include "chr.h"

void logout(int sockfd)
{
    char sql[BUFSIZ];

    sprintf(sql, "update userinfo set socket=-1, serv_type=0, online=0,"
        " chat_id=0, chat_fd=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

static int  serv_type = 0;

int get_serv_type(char *buf, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!(row = mysql_fetch_row(res))) {  /*  登陆或注册  */
        if (serv_type == LOGIN || serv_type == REGISTER)
            return serv_type;
        serv_type = atoi(buf);
        return 0;
    }
    mysql_free_result(res);

    if (!atoi(row[4])) {
        serv_type = atoi(buf);
        if (serv_type < CHPASSWD || serv_type > KICK) {
            sprintf(sql, "update userinfo set serv_type=0 where "
                "socket=%d and online=1;", sockfd);
            Mysql_query(con, sql);
        } else {
            sprintf(sql, "update userinfo set serv_type=%d where "
                "socket=%d and online=1;", serv_type, sockfd);
            Mysql_query(con, sql);
        }
        serv_type = 0;
    } else
        serv_type = atoi(row[4]);

    return serv_type;
}

void divide(char *buf, char *s)
{
    char    *ptr;

    ptr = buf;
    while (isspace(*ptr))
        ptr++;
    while (*ptr && !isspace(*ptr))
        *s++ = *ptr++;
    *s = '\0';
}

void check_login(char *buf, int sockfd)
{
    enum        {NAME, PASSWD};
    char        name[MAXNAME];
    char        passwd[MAXPASSWD];
    char        sql[BUFSIZ];
    MYSQL_RES   *res;

    divide(buf, name);
    divide(buf + strlen(name), passwd);

    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {
        Writen(sockfd, "username dosen't exist\n", 24);
        return;
    }
    mysql_free_result(res);

    sprintf(sql, "select * from userinfo where passwd='%s';", passwd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {
        Writen(sockfd, "wrong password\n", 15);
        return;
    }
    mysql_free_result(res);
    serv_type = 0;    /*  表示登陆成功  */

    /*  初始化用户信息  */
    sprintf(buf, "update userinfo set socket=%d, online=%d, serv_type=%d "
            "where name='%s';", sockfd, 1, 0, name);
    Mysql_query(con, buf);
    Writen(sockfd, "login successfully\n", 19);

    /*  查询并转发离线消息  */
}

void check_register(char *buf, int sockfd)
{
    enum        {NAME, PASSWD};
    char        sql[BUFSIZ];
    char        name[MAXNAME];
    char        passwd[MAXPASSWD];
    MYSQL_RES   *res;

    divide(buf, name);
    divide(buf + strlen(name), passwd);

    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {
        Writen(sockfd, "username already exist\n", 23);
        return;
    }
    mysql_free_result(res);
    serv_type = 0;

    /*  存储新注册用户的信息  */
    sprintf(sql, "insert into userinfo (name, socket, passwd, serv_type, online) "
        "values('%s', %d, '%s', %d, %d);", name, sockfd, passwd, 0, 1);
    Mysql_query(con, sql);
    Writen(sockfd, "registered successfully\n", 24);
}

void add_friend(char *name, int sockfd, int serv_type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   myrow, frirow;
    char        sql[BUFSIZ];
    int         frifd, online;

    /*  查找自己的id  */
    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    myrow = mysql_fetch_row(res);
    mysql_free_result(res);

    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);

    if (!mysql_num_rows(res)) {
        Writen(sockfd, "username dosen't exist\n", 23);
        return;
    }
    frirow = mysql_fetch_row(res);
    mysql_free_result(res);

    if ((online = atoi(frirow[5]))) {   /*  对方在线  */
        frifd = atoi(frirow[2]);
        sprintf(sql, "%s asks to be friend (y or n)\n", name);
        Writen(frifd, sql, strlen(sql));

        Readn(frifd, sql, sizeof(sql));
        if (strcmp(sql, "y") == 0) {    /*  对方同意  */
            sprintf(sql, "insert into friend (myid, frid) values(%d, %d)", 
                atoi(myrow[0]), atoi(frirow[0]));
            Mysql_query(con, sql);
            Writen(sockfd, "you have successfully become good friends\n", 42);
            Writen(frifd, "you have successfully become good friends\n", 42);
        } else if (strcmp(sql, "n") == 0) {
            Writen(sockfd, "I'm sorry the other refused your request\n", 41);
            return;
        } else {
            Writen(frifd, "wrong command\n", 14);
            return;
        }
    } else {   /*  对方不在线, 存为离线消息  */
        sprintf(sql, "insert into message (msg, myid, frid, serv_type) values"
            "('%s asks to be friend (y or n)\n', %d, %d, %d)", name, atoi(myrow[0]), 
            atoi(frirow[0]), serv_type); 
        Mysql_query(con, sql);
        Writen(sockfd, "I'm sorry they're not on line\n", 30);
    }
}

void private_chat(char *msg, int sockfd, int serv_type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   myrow, frirow;
    int         frifd;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    myrow = mysql_fetch_row(res);
    mysql_free_result(res);

    if (!atoi(myrow[6])) {   /*  msg中存放的是聊天对象  */
        sprintf(sql, "select * from userinfo where name='%s'", msg);
        Mysql_query(con, sql);
        res = mysql_store_result(con);
        frirow = mysql_fetch_row(res);
        mysql_free_result(res);

        sprintf(sql, "update userinfo set serv_type=%d, chat_id=%d, chat_fd=%d "
            "where name='%s'", serv_type, atoi(myrow[0]), atoi(myrow[2]), msg);    
        Mysql_query(con, sql);
        sprintf(sql, "update userinfo set chat_id=%d, chat_fd=%d  where socket=%d", 
            atoi(frirow[0]), atoi(frirow[2]), sockfd);
        Mysql_query(con, sql);
    } else {   /*  转发消息  */
        frifd = atoi(myrow[7]);    /*  获取好友的socket  */
        /*  判断好友是否在线  */
        sprintf(sql, "select * from userinfo where socket=%d", frifd);
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        frirow = mysql_fetch_row(res);

        if (!atoi(frirow[5])) {   /*  好友离线，缓存为离线消息  */
            strcat(msg, "\n");
            sprintf(sql, "insert into message (msg, myid, frid, serv_type) values"
                "('%s', %d, %d, %d)", msg, atoi(myrow[0]), atoi(frirow[0]), serv_type); 
            Mysql_query(con, sql);
            return;
        }

        if (strcmp(msg, "\\q") == 0) {   /*  聊天结束  */
            sprintf(sql, "update userinfo set serv_type=0, chat_id=0, "
                "chat_fd=0 where socket=%d", sockfd);    
            Mysql_query(con, sql);
            sprintf(sql, "update userinfo set serv_type=0, chat_id=0, "
                "chat_fd=0 where socket=%d", frifd);    
            Mysql_query(con, sql);
            return;
        } else {
            strcat(msg, "\n");
            Writen(frifd, msg, strlen(msg));
        }
    }
}
