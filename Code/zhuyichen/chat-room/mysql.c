#include "chr.h"

MYSQL *initmysql(void)
{
    MYSQL *con;

    if ((con = mysql_init(NULL)) == NULL)
        err_quit("%s", mysql_error(con));

    if (mysql_real_connect(con, "localhost", "yaoer", "12965si995",
            "chatroom", 0, NULL, 0) == NULL)
        err_quit("%s", mysql_error(con));
    return con;
}

void Mysql_query(MYSQL *con, const char *str)
{
    if (mysql_query(con, str))
        err_quit("%s", mysql_error(con));
}

MYSQL_RES *Mysql_store_result(MYSQL *con)
{
    MYSQL_RES *res;

    if ((res = mysql_store_result(con)) == NULL)
        err_quit("%s", mysql_error(con));
    return res;
}
