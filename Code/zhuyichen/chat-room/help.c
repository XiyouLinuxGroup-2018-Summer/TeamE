#include "chr.h" 

void help(int sockfd)
{
    static char buf[] = {
       "===========================================================================\n"
       ">  service types:                                                         <\n"
       ">  1.login                    [usage:  <1\\n> <username password\\n>]       <\n"
       ">  2.register                 [usage:  <2\\n> <username password\\n>]       <\n"
       ">  3.change password          [usage:  <3\\n> <userid newpassword\\n>]      <\n"
       ">  4.add friend               [usage:  <4\\n> <friendname\\n>]              <\n"
       ">  5.del friend               [usage:  <5\\n> <friendname\\n>]              <\n"
       ">  6.private chat             [usage:  <6\\n> <friendname\\n>]              <\n"
       ">  7.group chat               [usage:  <7\\n> <groupname\\n>]               <\n"
       ">  8.shield message           [usage:  <8\\n> <username or groupname\\n>]   <\n"
       ">  9.add group                [usage:  <9\\n> <invitee groupname\\n>]       <\n"
       "> 10.creat group              [usage: <10\\n> <groupname\\n>]               <\n"
       "> 11.leave group              [usage: <11\\n> <groupname\\n>]               <\n"
       "> 12.send file                [usage: <12\\n> <filename receiver\\n>]       <\n"
       "> 13.set administrator        [usage: <13\\n> <groupmember\\n>]             <\n"
       ">'\\q': exit an ongoing service <1~13 above>                               <\n"
       "> *********************************************************************** <\n"
       "> when no service is available, you can execute the following commands.   <\n"
       ">'\\h': show service list                                                  <\n"
       ">'\\f': show friends list                                                  <\n"
       ">'\\c': clean screen                                                       <\n"
       "===========================================================================\n"
    };
    Writen(sockfd, buf, strlen(buf));
}

void clean(int sockfd)
{
    char buf[] = "\033c";
    Writen(sockfd, buf, strlen(buf));
}

/*  show_frilist函数：显示出好友列表  */
void show_frilist(int sockfd)
{
    MYSQL_RES   *res;
    MYSQL_ROW   prow, qrow;
    char        sql[BUFSIZ];
    int         myid;

    sprintf(sql, "select * from userinfo where socket=%d;", sockfd);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);
    if (!mysql_num_rows(res)) {
        sprintf(sql, "Please login first\n");
        Writen(sockfd, sql, strlen(sql));
        return;
    }
    prow = mysql_fetch_row(res);
    mysql_free_result(res);

    myid = atoi(prow[0]);
    sprintf(sql, "select * from friend where myid=%d or frid=%d;", myid, myid);
    Mysql_query(con, sql);
    res = Mysql_store_result(con);

    if (mysql_num_rows(res)) {
        while ((prow = mysql_fetch_row(res))) {
            if (myid != atoi(prow[0])) {
                sprintf(sql, "select * from userinfo where id=%d", atoi(prow[0]));
                Mysql_query(con, sql);
                res = Mysql_store_result(con);
                qrow = mysql_fetch_row(res);
            } else if (myid != atoi(prow[1])) {
                sprintf(sql, "select * from userinfo where id=%d", atoi(prow[1]));
                Mysql_query(con, sql);
                res = Mysql_store_result(con);
                qrow = mysql_fetch_row(res);
            }
            strcpy(sql, qrow[1]);
            strcat(sql, "\n");
            Writen(sockfd, sql, strlen(sql));
        }
    } else {   /*  还没有好友  */
        sprintf(sql, "I'm sorry you don't have any friends yet\n");
        Writen(sockfd, sql, strlen(sql));
    }
}
