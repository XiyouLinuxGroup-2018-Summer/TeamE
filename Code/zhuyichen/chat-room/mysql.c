#include "chr.h"

MYSQL *initmysql(void)
{
    MYSQL *con;

    if ((con = mysql_init(NULL)) == NULL)
        err_sys("mysql_init error");

    if (mysql_real_connect(con, "localhost", "yaoer", "12965si995",
            "chatroom", 0, NULL, 0) == NULL)
        err_sys("mysql_real_connect error");
    return con;
}

void Mysql_query(MYSQL *con, const char *str)
{
    if (mysql_query(con, str))
        err_sys("mysql_query error");
}
