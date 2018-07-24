#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/*  chmod命令的简化版本  */
int main(int argc, char **argv)
{
    int     mode;
    int     mode_u;    /*  所有者权限  */ 
    int     mode_g;    /*  用户组权限  */
    int     mode_o;    /*  其他用户权限  */
    char    *path;

    if (argc != 3) {
        printf("usage: %s mode [file]\n", argv[0]);
        exit(1);
    }

    mode = atoi(argv[1]);
    if (mode > 777 || mode < 0) {
        printf("mode error\n");
        exit(1);
    }
    path = argv[2];

    mode_u = mode / 100;
    mode_g = mode / 10 % 10;
    mode_o = mode % 10;
    mode = mode_u * 64 + mode_g * 8 + mode_o;    /*  将mode转换为8进制  */

    if (chmod(path, mode) < 0) {
        printf("chmod error\n");
        exit(1);
    }
    exit(0);
}
