#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int mode(char *);

/*  创建一个文件  */
int main(int argc, char **argv)
{
    int fd;

    if (argc != 3) {
        printf("usage: %s [file] [mode]\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_CREAT | O_EXCL, mode(argv[2]))) < 0) {
        printf("error: %s\n", strerror(errno));     /*  已存在  */
        exit(1);
    }
    exit(0);
}

/*  mode函数：将文件访问模式转换为8进制  */
int mode(char *md)
{
    int     mode;
    int     mode_u;    /*  所有者权限  */ 
    int     mode_g;    /*  用户组权限  */
    int     mode_o;    /*  其他用户权限  */

    mode = atoi(md);
    if (mode > 777 || mode < 0) {
        printf("mode error\n");
        exit(1);
    }

    mode_u = mode / 100;
    mode_g = mode / 10 % 10;
    mode_o = mode % 10;
    mode = mode_u * 64 + mode_g * 8 + mode_o;    /*  将mode转换为8进制  */
    return mode;
}
