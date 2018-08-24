#include "chr.h"

/*  logout函数：用户下线时的一些清理工作  */
void logout(int sockfd)
{
    char sql[BUFSIZ];

    sprintf(sql, "delete from usertmp where socket=%d;", sockfd);
    mysql_query(con, sql);
    sprintf(sql, "update userinfo set socket=-1, serv_type=0, online=0,"
        " chat_id=0, chat_fd=0, filename=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

/*
 * 这里设置为全局变量的原因是为了处理登录和注册时的特殊情况
 */

/*  get_serv_type函数：返回当前用户的服务状态  */
int get_serv_type(char *buf, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];
    int         serv_type;   

    sprintf(sql, "select * from usertmp where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {   /*  登录或注册  */
        row = mysql_fetch_row(res);
        mysql_free_result(res);
        serv_type = atoi(row[1]);
        return serv_type;
    }
    mysql_free_result(res);

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {  /*  登录或注册  */
        serv_type = atoi(buf);
        if (serv_type == LOGIN || serv_type == REGISTER) {
            sprintf(sql, "insert into usertmp (socket, serv_type) values(%d, %d);", 
                sockfd, serv_type);
            Mysql_query(con, sql);
        }
        mysql_free_result(res);
        return 0;
    }
    row = mysql_fetch_row(res);
    mysql_free_result(res);

    if (!atoi(row[4])) {    /*  无服务状态  */
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
void divide(char *buf, char *s, int lim)
{
    char    *ptr, *ps;

    ptr = buf;
    while (isspace(*ptr))
        ptr++;
    ps = s;
    while (s < ps + lim && *ptr && !isspace(*ptr))
        *s++ = *ptr++;
    *s = '\0';
}

/*  check_login函数：检查用户登录  */
void check_login(char *buf, int sockfd)
{
    char        name[MAXIDENT];
    char        passwd[MAXIDENT];
    char        sql[BUFSIZ];
    MYSQL_RES   *res;

    divide(buf, name, MAXIDENT);
    if (!*(buf + strlen(name))) {
        sprintf(sql, "Wrong format\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    divide(buf + strlen(name), passwd, MAXIDENT);

    /*  检查用户名  */
    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {
        sprintf(sql, "username <%s> doesn't exit\n", name);
        Writen(sockfd, sql, strlen(sql));
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);

    sprintf(sql, "select * from userinfo where name='%s' and online=1;", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {    /*  该用户已登录  */
        sprintf(sql, "username <%s> logged in\n", name);
        Writen(sockfd, sql, strlen(sql));
        mysql_free_result(res);
        return;
    }

    /*  检查密码  */
    sprintf(sql, "select * from userinfo where name='%s' and passwd='%s';", 
        name, passwd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {
        strcpy(sql, "wrong password\n");
        Writen(sockfd, sql, strlen(sql));
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);

    sprintf(sql, "delete from usertmp where socket=%d;", sockfd);
    mysql_query(con, sql);

    /*  初始化用户信息  */
    sprintf(buf, "update userinfo set socket=%d, online=1, serv_type=0, chat_id=0, "
        "chat_fd=0 where name='%s';", sockfd, name);
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
    char        name[MAXIDENT];
    char        passwd[MAXIDENT];

    divide(buf, name, MAXIDENT);
    if (!*(buf + strlen(name))) {
        sprintf(sql, "Wrong format\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    divide(buf + strlen(name), passwd, MAXIDENT);

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

    /*  存储新注册用户的信息  */
    sprintf(sql, "insert into userinfo (name, socket, passwd, serv_type, online, chat_id, chat_fd) "
        "values('%s', %d, '%s', 0, 1, 0, 0);", name, sockfd, passwd);
    Mysql_query(con, sql);

    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_free_result(res);

    sprintf(sql, "delete from usertmp where socket=%d;", sockfd);
    Mysql_query(con, sql);

    /*  用户id用于找回密码  */
    sprintf(sql, "registered successfully\nyour id = %d (for retrieving password)\n",
        atoi(row[0]));
    Writen(sockfd, sql, strlen(sql));
}

/*  change_passwd函数：修改密码  */
void change_passwd(char *buf, int sockfd)
{
    char        sql[BUFSIZ];
    char        id[MAXIDENT];
    char        passwd[MAXIDENT];
    MYSQL_RES   *res;
    MYSQL_ROW   row;

    divide(buf, id, MAXIDENT);
    if (!*(buf + strlen(id))) {
        sprintf(sql, "Wrong format\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    divide(buf + strlen(id), passwd, MAXIDENT);

    sprintf(sql, "select * from userinfo where socket=%d", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_num_rows(res);

    if (strcmp(id, row[0]) == 0) {   /*  输入id符合  */
        sprintf(sql, "update userinfo set passwd='%s' where id=%d", passwd, atoi(id));
        Mysql_query(con, sql);
        Writen(sockfd, "Password changed successfully\n", 30);
    } else {    /*  id错误  */
        strcpy(sql, "Wrong id\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);    
    Mysql_query(con, sql);
}

/*  add_friend函数：添加好友  */
void add_friend(char *buf, int sockfd, int type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   prow, qrow;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd); 
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    prow = mysql_fetch_row(res);

    if (!atoi(prow[7])) {   /*  buf中存放的是待加好友名字  */
        sprintf(sql, "select * from userinfo where name='%s';", buf); 
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (!mysql_num_rows(res)) {   /*  待加好友不存在  */
            sprintf(sql, "username <%s> doesn't exist\n", buf);
            Writen(sockfd, sql, strlen(sql));
            mysql_free_result(res);
            return;
        }
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        sprintf(sql, "select * from friend where myid=%d or frid=%d;", 
            atoi(prow[0]), atoi(qrow[0])); 
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (mysql_num_rows(res)) {    /*  已拥有该好友  */
            sprintf(sql, "You're friends and don't have to add him again\n");
            Writen(sockfd, sql, strlen(sql));
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);

        sprintf(sql, "select * from userinfo where name='%s' and online=1;", buf); 
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (!mysql_num_rows(res)) {   /*  待加好友不在线  */
            sprintf(sql, "<%s> is not online\n", buf);
            Writen(sockfd, sql, strlen(sql));

            /*  缓存为离线消息  */
            sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                "values('%s requests to be friend', %d, %d, %d, 0)", 
                buf, atoi(prow[0]), atoi(qrow[0]), type);
            Mysql_query(con, sql);
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);

        if (atoi(qrow[4])) {   /*  待加好友处于非空闲状态  */
            sprintf(sql, "<%s> is busy\n", buf);
            Writen(sockfd, sql, strlen(sql));

            /*  暂时缓存起来  */
            sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                "values('<%s> requests to be friend', %d, %d, %d, 1)", 
                prow[1], atoi(prow[0]), atoi(qrow[0]), type);
            Mysql_query(con, sql);
            mysql_free_result(res);
            return;
        }

        sprintf(sql, "update userinfo set serv_type=%d, chat_id=%d, chat_fd=%d "
            "where name='%s';", type, atoi(prow[0]), atoi(prow[2]), buf);
        Mysql_query(con, sql);
        sprintf(sql, "<%s> requests to be friend\n", qrow[1]);
        Writen(sockfd, sql, strlen(sql));
    } else {    /*  判断回复消息  */
        sprintf(sql, "select * from userinfo where id=%d;", atoi(prow[6])); 
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        if (strcmp(buf, "y") == 0) {
            sprintf(sql, "insert into friend (myid, frid) values(%d, %d)", 
                atoi(qrow[0]), atoi(prow[0]));
            Mysql_query(con, sql);
            sprintf(sql, "Your request was granted by <%s>\n", prow[1]);
            Writen(atoi(qrow[2]), sql, strlen(sql));
        } else if (strcmp(buf, "n") == 0) {
            sprintf(sql, "Your request was rejected by <%s>\n", prow[1]);
            Writen(atoi(qrow[2]), sql, strlen(sql));
        } else {
            sprintf(sql, "Wrong command\n");
            Writen(atoi(prow[2]), sql, strlen(sql));
            return;
        }

        /*  清理工作  */
        sprintf(sql, "update userinfo set serv_type=0, chat_id=0, chat_fd=0 "
            "where name='%s';", prow[1]);    
        Mysql_query(con, sql);
        sprintf(sql, "update userinfo set serv_type=0 where name='%s';", qrow[1]);    
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
    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_free_result(res);

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
void private_chat(char *buf, int sockfd, int type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   prow, qrow;
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    prow = mysql_fetch_row(res);
    mysql_free_result(res);

    if (!atoi(prow[6])) {   /*  buf中存放的是好友名字  */
        sprintf(sql, "select * from userinfo where name='%s';", buf);
        Mysql_query(con, sql);
        res= Mysql_store_result(con);
        if (!mysql_num_rows(res)) {    /*  该好友不存在  */
            sprintf(sql, "The chat object name doesn't exist\n");
            Writen(sockfd, sql, strlen(sql));
            return;
        }
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        sprintf(sql, "select * from userinfo where name='%s' and online=1;", buf); 
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (!mysql_num_rows(res)) {   /*  该好友不在线  */
            sprintf(sql, "<%s> is not online\n", buf);
            Writen(sockfd, sql, strlen(sql));
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);

        if (atoi(qrow[4])) {   /*  该好友处于非空闲状态  */
            sprintf(sql, "<%s> is busy\n", buf);
            Writen(sockfd, sql, strlen(sql));
            return;
        }

        /*  初始化聊天设置  */
        sprintf(sql, "update userinfo set serv_type=%d, chat_id=%d, chat_fd=%d "
            "where name='%s';", type, atoi(prow[0]), atoi(prow[2]), buf);    
        Mysql_query(con, sql);
        sprintf(sql, "update userinfo set chat_id=%d, chat_fd=%d  where socket=%d;", 
            atoi(qrow[0]), atoi(qrow[2]), sockfd);
        Mysql_query(con, sql);
    } else {   /*  转发消息  */
        /*  判断好友是否在线  */
        sprintf(sql, "select * from userinfo where id=%d and online=1;", atoi(prow[6]));
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (!mysql_num_rows(res)) {    /*  对方不在线  */
            strcat(buf, "\n");
            sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                "values('%s', %d, %d, %d, 0);", buf, atoi(prow[0]), atoi(prow[6]), type); 
            Mysql_query(con, sql);
            mysql_free_result(res);
            return;
        }
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        if (atoi(qrow[4]) != 0 && atoi(qrow[4]) != PRVC) {  /*  非空闲状态，发出一条消息通知  */
            sprintf(sql, "You have a new message from <%s>\n", prow[1]);
            Writen(atoi(qrow[2]), sql, strlen(sql));
            strcat(buf, "\n");
            sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
                "values('%s', %d, %d, %d, 1);", buf, atoi(prow[0]), atoi(prow[6]), type); 
            Mysql_query(con, sql);
            return;
        }

        strcat(buf, "\n");
        Writen(atoi(qrow[2]), buf, strlen(buf));
        sprintf(sql, "insert into message (msg, myid, frid, serv_type, readstatus) "
            "values('%s', %d, %d, %d, 2);", buf, atoi(prow[0]), atoi(prow[6]), type); 
        Mysql_query(con, sql);
    }
}

/*  group_chat函数：多人群聊  */
void group_chat(char *buf, int sockfd, int type)
{
    MYSQL_RES   *pres, *qres;
    MYSQL_ROW   row;
    char        sql[BUFSIZ];
    char        sender[MAXIDENT];
    int         sendid, grpid = 0;

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    pres = Mysql_store_result(con);
    row = mysql_fetch_row(pres);
    strcpy(sender, row[1]);
    sendid = atoi(row[0]);
    mysql_free_result(pres);

    if (!atoi(row[6])) {   /*  buf中为群名  */
        sprintf(sql, "select * from chat_group where name='%s';", buf);
        Mysql_query(con, sql);
        pres = Mysql_store_result(con);
        if (!mysql_num_rows(pres)) {   /*  该群不存在  */
            sprintf(sql, "Group <%s> doesn't exist\n", buf);
            Writen(sockfd, sql, strlen(sql));
            mysql_free_result(pres);
            return;
        }
        row = mysql_fetch_row(pres);
        mysql_free_result(pres);

        grpid = atoi(row[0]);
        sprintf(sql, "update userinfo set chat_id=%d where socket=%d;", atoi(row[0]), sockfd);    
        Mysql_query(con, sql);
    } else {   /*  将消息广播给所有在线群员  */
        /*  查找群名  */
        sprintf(sql, "select * from chat_group where id=%d;", atoi(row[6]));
        Mysql_query(con, sql);
        pres = Mysql_store_result(con);
        row = mysql_fetch_row(pres);
        mysql_free_result(pres);

        sprintf(sql, "select * from grpmembers where grpname='%s';", row[1]);
        Mysql_query(con, sql);
        pres = Mysql_store_result(con);
        while ((row = mysql_fetch_row(pres))) {
            sprintf(sql, "select * from userinfo where name='%s' and online=1 and serv_type=%d;",
                row[0], type);
            Mysql_query(con, sql);
            qres = Mysql_store_result(con);
            if (mysql_num_rows(qres)) {   /*  如果在线且处于群聊状态  */
                row = mysql_fetch_row(qres);
                mysql_free_result(qres);
                sprintf(sql, "<%s>\n", sender);
                strcat(buf, sql);
                if (strcmp(row[1], sender) != 0)
                    Writen(atoi(row[2]), buf, strlen(buf));

                sprintf(sql, "insert into userinfo (msg, myid, frid, serv_type, readstatus) values"
                    "(%s, %d, %d, %d, 2);", buf, sendid, grpid, type);
                Mysql_query(con, sql);
            } else {   /*  存为离线消息  */

            }
        }
    }
}

/*  creat_group函数：创建一个群  */
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
    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_free_result(res);

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

/*  add_group函数：被邀请入群  */
void add_group(char *buf, int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   row;
    char        name[MAXIDENT];    /*  被邀请人  */
    char        groupname[MAXIDENT];
    char        sql[BUFSIZ];

    divide(buf, name, MAXIDENT);
    if (!*(buf + strlen(name))) {
        sprintf(sql, "Wrong format\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    divide(buf + strlen(name), groupname, MAXIDENT);

    /*  检查被邀请人是否存在  */
    sprintf(sql, "select * from userinfo where name='%s';", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {  /*  被邀请人不存在  */
        sprintf(sql, "The invitee <%s> doesn't exist\n", name);
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    row = mysql_fetch_row(res);
    mysql_free_result(res);

    /*  检查被邀请人是否已是群成员  */
    sprintf(sql, "select * from grpmembers where name='%s' and grpname='%s';",
        name, groupname);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {   /*  被邀请人已在该群中  */
        sprintf(sql, "The invitee is already in the group and doesn't need to be invited again\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
   
    /*  将被邀请人添加到表grpmembers中  */
    sprintf(sql, "insert into grpmembers (name, grpname, limits) values('%s', '%s', 0);", 
        name, groupname);
    Mysql_query(con, sql);

    /*  成功加入该群  */
    sprintf(sql, "select * from userinfo where name='%s' and online=1", name);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {   /*  被邀请人不在线  */
        sprintf(sql, "insert into message (msg, frid, readstatus) values('You have successfully "
            "joined the group', %d, 0)", atoi(row[2]));
        Mysql_query(con, sql);
    } else {
        sprintf(sql, "You have successfully joined the group <%s>\n", groupname);
        Writen(atoi(row[2]), sql, strlen(sql));
    }

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

/*  leave_group函数：退出群  */
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
        strcpy(sql, "The group doesn't exist\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    mysql_free_result(res);

    /*  找到退群的成员的名字  */
    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    row = mysql_fetch_row(res);
    mysql_free_result(res);

    /*  判断该成员是否是群主  */
    sprintf(sql, "select * from grpmembers where name='%s' and limits=2;", row[1]);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (mysql_num_rows(res)) {   /*  是群主，解散该群  */
        /*  删除所有群成员  */
        sprintf(sql, "delete from grpmembers where grpname='%s';", name);
        Mysql_query(con, sql);

        /* 从表chat_group中删除该群名  */
        sprintf(sql, "delete from chat_group where name='%s';", name);
        Mysql_query(con, sql);
        mysql_free_result(res);

        strcpy(sql, "The group has been dissolved\n");
        Writen(sockfd, sql, strlen(sql));
    } else {   /*  不是群主，仅移除该成员  */
        sprintf(sql, "delete from grpmembers where name='%s';", name);
        Mysql_query(con, sql);

        strcpy(sql, "You have successfully in leaving the group\n");
        Writen(sockfd, sql, strlen(sql));
    }

    /*  清理工作  */
    sprintf(sql, "update userinfo set serv_type=0 where socket=%d;", sockfd);
    Mysql_query(con, sql);
}

void send_file(char *buf, int sockfd, int type)
{
    MYSQL_RES   *res;
    MYSQL_ROW   prow, qrow;
    char        filename[BUFSIZ];
    char        receiver[MAXIDENT];
    char        sql[BUFSIZ];

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    prow = mysql_fetch_row(res);

    if (!atoi(prow[7])) {
        if (atoi(prow[6])) {   /*  判断回复消息  */
            sprintf(sql, "select * from userinfo where id=%d;", atoi(prow[6]));
            Mysql_query(con, sql);
            res = Mysql_store_result(con);
            qrow = mysql_fetch_row(res);
            mysql_free_result(res);

            if (strcmp(buf, "y") == 0) {
                sprintf(sql, "<%s> Agree to receive and senting...\n", prow[1]);
                Writen(atoi(qrow[2]), sql, strlen(sql));
                sprintf(sql, "ys\r%s", prow[8]);
                Writen(atoi(qrow[2]), sql, strlen(sql));
                sprintf(sql, "yr\r%s", prow[8]);
                Writen(atoi(prow[2]), sql, strlen(sql));
                sprintf(sql, "update userinfo set chat_fd=%d where id=%d;", atoi(prow[2]), atoi(qrow[0]));
                Mysql_query(con, sql);
            } else if (strcmp(buf, "n") == 0) {
                sprintf(sql, "<%s> Refuse to receive\n", prow[1]);
                Writen(atoi(qrow[2]), sql, strlen(sql));
                sprintf(sql, "n\r");
                Writen(atoi(qrow[2]), sql, strlen(sql));
                sprintf(sql, "update userinfo set serv_type=0, chat_id=0 where id=%d;", atoi(prow[0]));
                Mysql_query(con, sql);
            } else {
                sprintf(sql, "Wrong command\n");
                Writen(atoi(qrow[2]), sql, strlen(sql));
            }
            return;
        }

        divide(buf, filename, BUFSIZ);
        if (!*(buf + strlen(filename))) {
            sprintf(sql, "Wrong format\n");
            Writen(sockfd, sql, strlen(sql));
            return;
        }
        divide(buf + strlen(filename), receiver, MAXIDENT);

        sprintf(sql, "select * from userinfo where name='%s';", receiver);
        Mysql_query(con, sql);
        res = Mysql_store_result(con);
        if (!mysql_num_rows(res)) {    /*  接收人不存在  */
            sprintf(sql, "receiver doesn't exist\n");
            Writen(sockfd, sql, strlen(sql));
            mysql_free_result(res);
            return;
        }
        qrow = mysql_fetch_row(res);
        mysql_free_result(res);

        if (atoi(qrow[5])) {    /*  接收人在线  */
            sprintf(sql, "Whether to receive the file <%s> from <%s> <y or n>?\n", 
                filename, receiver);
            Writen(atoi(qrow[2]), sql, strlen(sql));

            sprintf(sql, "update userinfo set serv_type=%d, chat_id=%d, filename='%s' where name='%s';", 
                type, atoi(prow[0]), filename, receiver);
            Mysql_query(con, sql);
        } else {   /*  不在线  */
            sprintf(sql, "I'm Sorry receiver <%s> is not online\n", receiver);
            Writen(sockfd, sql, strlen(sql));
        }
    } else    
        Writen(atoi(prow[7]), buf, strlen(buf));
}
