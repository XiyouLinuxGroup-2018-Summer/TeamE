#include "chr.h"

/*  logout函数：用户下线时的一些清理工作  */
void logout(int sockfd)
{
    char sql[BUFSIZ];

    sprintf(sql, "update userinfo set socket=-1, serv_type=0, online=0,"
        " chat_id=0, chat_fd=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

/*  get_sql_row函数(一个简单的封装)：返回数据库中某一条记录  */
MYSQL_ROW get_sql_row(void)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;

    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_free_result(res);
    return row;
}

/*
 * 这里设置为全局变量的原因是为了处理登录和注册时的特殊情况
 */
static int serv_type = 0;   

/*  get_serv_type函数：返回当前用户的服务状态  */
int get_serv_type(char *buf, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {  /*  登录或注册  */
        if (serv_type == LOGIN || serv_type == REGISTER)
            return serv_type;
        serv_type = atoi(buf);
        return 0;
    }
    row = mysql_fetch_row(res);
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

/*  divide函数：分离以空白符分隔的字符串  */
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

/*  check_login函数：检查用户登录  */
void check_login(char *buf, int sockfd)
{
    char        name[MAXNAME];
    char        passwd[MAXPASSWD];
    char        sql[BUFSIZ];
    MYSQL_RES   *res;

    divide(buf, name);
    divide(buf + strlen(name), passwd);

    /*  检查用户名  */
    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {    /*  用户名不存在  */
        strcpy(sql, "username doesn't exit\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    mysql_free_result(res);

    /*  检查密码  */
    sprintf(sql, "select * from userinfo where passwd='%s';", passwd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {   /*  密码错误  */
        strcpy(sql, "wrong password\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    mysql_free_result(res);
    serv_type = 0;    /*  标记为登录成功  */

    /*  初始化用户信息  */
    sprintf(buf, "update userinfo set socket=%d, online=%d, serv_type=%d "
            "where name='%s';", sockfd, 1, 0, name);
    Mysql_query(con, buf);
    strcpy(sql, "login successfully\n");
    Writen(sockfd, sql, strlen(sql));

    /*  查询并转发离线消息  */
}

/*  check_register函数：检查用户注册  */
void check_register(char *buf, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];
    char        name[MAXNAME];
    char        passwd[MAXPASSWD];

    divide(buf, name);
    divide(buf + strlen(name), passwd);

    /*  检查用户名  */
    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {   /*  用户名已存在  */
        strcpy(sql, "username already exist\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    mysql_free_result(res);
    serv_type = 0;    /*  标记为注册成功  */

    /*  存储新注册用户的信息  */
    sprintf(sql, "insert into userinfo (name, socket, passwd, serv_type, online) "
        "values('%s', %d, '%s', %d, %d);", name, sockfd, passwd, 0, 1);
    Mysql_query(con, sql);

    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    row = get_sql_row();
    /*  用户id用于找回密码  */
    sprintf(sql, "registered successfully\nyour id = %d (for retrieving password)\n",
        atoi(row[0]));
    Writen(sockfd, sql, strlen(sql));
}

/*  change_passwd函数：修改密码  */
void change_passwd(char *buf, int sockfd)
{
    MYSQL_ROW   row;
    char        sql[BUFSIZ];
    char        id[32];
    char        passwd[MAXPASSWD];

    divide(buf, id);
    divide(buf + strlen(id), passwd);

    sprintf(sql, "select * from userinfo where socketd=%d", sockfd);
    Mysql_query(con, sql);
    row = get_sql_row();

    if (strcmp(id, row[0]) == 0) {   /*  输入id符合  */
        sprintf(sql, "update userinfo set passwd='%s' where id=%d", passwd, atoi(id));
        Mysql_query(con, sql);
        Writen(sockfd, "Password changed successfully\n", 30);
    } else {    /*  id错误  */
        strcpy(sql, "Wrong id\n");
        Writen(sockfd, sql, strlen(sql));
    }

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);    
    Mysql_query(con, sql);
}

/*  add_friend函数：添加好友  */
void add_friend(char *buf, int sockfd, int serv_type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   prow, qrow;
    char        sql[BUFSIZ];

    /*  查看自己的chat_id或chat_fd  */
    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    prow = get_sql_row();

    if (!atoi(prow[6])) {   /*  buf中为待加好友名字  */
        /*  先判断用户名是否存在  */
        sprintf(sql, "select * from userinfo where name='%s';", buf);   
        Mysql_query(con, sql);
        res = Mysql_store_result(con);

        if (!mysql_num_rows(res)) {   /*  用户名不存在  */
            strcpy(sql, "username doesn't exist\n");
            Writen(sockfd, sql, strlen(sql));
            return;
        }

        /*  再判断对方是否在线并且处于空闲状态  */
        sprintf(sql, "select * from userinfo where name='%s' and online=1;", buf);   
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        qrow = mysql_fetch_row(res);

        if (!mysql_num_rows(res) || atoi(qrow[4])) {   /*  不在线或非空闲状态  */
            sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                "values('%s requests to be friend (y or n)\n', %d, %d, %d, 0);", 
                prow[1], atoi(prow[0]), atoi(qrow[0]), serv_type);
            Mysql_query(con, sql);
            sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);
            Mysql_query(con, sql);
        }
        mysql_free_result(res);
        
        /*  发送添加请求  */
        strcpy(sql, "%s requests to be friend (y or n)\n");
        Writen(sockfd, sql, strlen(sql));

        /*  设置对方的chat_id和chat_fd设置为我的id和socket  */
        sprintf(sql, "update userinfo set chat_id=%d, chat_fd=%d where name='%s'",
            atoi(prow[0]), atoi(prow[2]), buf);
        Mysql_query(con, sql);
    } else {    /*  判断回复消息  */
        sprintf(sql, "select * from userinfo where id=%d and online=1", atoi(prow[6]));
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        if (strcmp(buf, "y\n") == 0) { 
            if (!mysql_num_rows(res)) {    /*  申请人不在线  */
                sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                    "values('you have successfully added as friends\n', 0, %d, 0, 0);", atoi(qrow[0]));
                Mysql_query(con, sql);
            }

            /*  更新表friend中的信息  */
            sprintf(sql, "update friend set myid=%d, frifd=%d;", atoi(qrow[0]), atoi(prow[0]));
            Mysql_query(con, sql);

            /*  成功添加为好友  */
            strcpy(sql, "you have successfully added as friends\n");
            Writen(atoi(qrow[2]), sql, strlen(sql));
            Writen(atoi(prow[2]), sql, strlen(sql));
        } else if (strcmp(buf, "n\n") == 0) {
            if (!mysql_num_rows(res)) {    /*  申请人不在线  */
                sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                    "values('I'm sorry, but your request was rejected\n', 0, %d, 0, 0);", atoi(qrow[0]));
                Mysql_query(con, sql);
            }

            /*  请求被对方拒绝  */
            strcpy(sql, "I'm sorry, but your request was rejected\n");
            Writen(atoi(qrow[2]), sql, strlen(sql));
        } else {    /*  回复有误  */
            strcpy(sql, "Reply is wrong\n");
            Writen(atoi(prow[2]), sql, strlen(sql));
            return;
        }

        /*  清理工作  */
        sprintf(sql, "update userinfo set serv_type=0, chat_id=0, chat_fd=0 "
            "where id=%d;", atoi(prow[0]));
        Mysql_query(con, sql);
        sprintf(sql, "update userinfo set serv_type=0 where id=%d;", atoi(qrow[0]));
        Mysql_query(con, sql);
    }
}

/*  del_friend函数：删除好友  */
void del_friend(char *name, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];
    
    /*  查到自己的id  */
    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    row = get_sql_row();

    /*  判断是否有该好友  */
    sprintf(sql, "select * from friend where myid=%d or frid=%d;", atoi(row[0]), atoi(row[0]));
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {   /*  不存在该好友  */
        strcpy(sql, "The friend doesn't exist\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    mysql_free_result(res);
    

    /*  删除该好友  */
    sprintf(sql, "delete from friend where myid=%d or frid=%d;", atoi(row[0]), atoi(row[0]));
    Mysql_query(con, sql);

    strcpy(sql, "The friend deleted successfully\n");
    Writen(sockfd, sql, strlen(sql));

    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);    
    Mysql_query(con, sql);
}

/*  private_chat函数：与好友私聊  */
void private_chat(char *msg, int sockfd, int serv_type)
{
    MYSQL_ROW   myrow, frirow;
    int         frifd;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    myrow = get_sql_row();

    if (!atoi(myrow[6])) {   /*  msg中存放的是聊天对象  */
        sprintf(sql, "select * from userinfo where name='%s';", msg);
        Mysql_query(con, sql);
        frirow = get_sql_row();

        sprintf(sql, "update userinfo set serv_type=%d, chat_id=%d, chat_fd=%d "
            "where name='%s';", serv_type, atoi(myrow[0]), atoi(myrow[2]), msg);    
        Mysql_query(con, sql);
        sprintf(sql, "update userinfo set chat_id=%d, chat_fd=%d  where socket=%d;", 
            atoi(frirow[0]), atoi(frirow[2]), sockfd);
        Mysql_query(con, sql);
    } else {   /*  转发消息  */
        frifd = atoi(myrow[7]);    /*  获取好友的socket  */
        /*  判断好友是否在线  */
        sprintf(sql, "select * from userinfo where socket=%d;", frifd);
        Mysql_query(con, sql);
        frirow = get_sql_row();

        if (!atoi(frirow[5])) {   /*  好友离线，缓存为离线消息  */
            strcat(msg, "\n");
            sprintf(sql, "insert into message (msg, myid, frid, serv_type) values"
                "('%s', %d, %d, %d);", msg, atoi(myrow[0]), atoi(frirow[0]), serv_type); 
            Mysql_query(con, sql);
            return;
        }

        strcat(msg, "\n");
        Writen(frifd, msg, strlen(msg));
    }
}

void creat_group(char *name, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];

    /*  在表chat_group中查询，是否该群名已存在  */
    sprintf(sql, "select * from chat_group where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);

    if (mysql_num_rows(res)) {   /*  该群名已存在  */
        strcpy(sql, "Sorry, group name repeated\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }

    /*  将群名加入表chat_group中  */
    sprintf(sql, "insert into chat_group (name, nmembers, onlines) "
        "values('%s', 1, 1);", name);
    Mysql_query(con, sql);

    /*  将群主的名字和群名加入表grpmembers中  */
    sprintf(sql, "select * from userinfo where socket=%d", sockfd);
    Mysql_query(con, sql);
    row = get_sql_row();
    sprintf(sql, "insert into grpmembers (name, grpname, limits) "
        "values('%s', '%s', 2);", row[1], name);
    Mysql_query(con, sql);

    /*  群已创建成功  */
    strcpy(sql, "The group has been created successfully\n");
    Writen(sockfd, sql, strlen(sql));

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

void leave_group(char *name, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];

    /*  查询该群是否存在  */
    sprintf(sql, "select * from chat_group where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {   /*  该群不存在  */

    }
    mysql_free_result(res);

    /*  找到退群的成员的名字  */
    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    row = get_sql_row();

    /*  判断该成员是否是群主  */
    sprintf(sql, "select * from grpmembers where name='%s' and limits=2;", row[1]);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {   /*  是群主，解散该群  */

    }

    /*  不是群主，从表grpmembers中删除该成员  */
    sprintf(sql, "delete from grpmembers where name='%s';", name);
    Mysql_query(con, sql);
    strcpy(sql, "You have successfully in leaving the group");
    Writen(sockfd, sql, strlen(sql));

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}
