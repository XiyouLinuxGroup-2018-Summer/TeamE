#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

#define FILE_MAX    256      /*  列出的最大文件数  */
#define FILE_ALL    0001     /*  -a  */
#define FILE_LON    0002     /*  -l  */
#define FILE_TIM    0004     /*  -t  */
#define FILE_SIZ    0010     /*  -S  */
#define FILE_UND    0020     /*  -h  */
#define FILE_REV    0040     /*  -r  */
#define FILE_REC    0100     /*  -R  */

static int option = 0;       /*  设置屏蔽位  */

int filecmp(const void *, const void *);
int timecmp(const void *, const void *);
int sizecmp(const void *, const void *);

void fsize_trans(off_t size);

void show_dir(char *pathname);
int get_width(int ws_col, char buf[][PATH_MAX + 1], int filenum);
void show_align(int ws_col, int width, char *pathname);
void show_attribute(struct stat statbuf);
void err_exit(char *fmt, ...);

/*
 * ls命令的简要实现，可用于处理[-la]参数 
 */
int main(int argc, char **argv)
{
    int c;
    /*  解析输入参数  */
    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0])) 
            switch (c) {
            case 'a':
                option |= FILE_ALL;
                break;
            case 'l':
                option |= FILE_LON;
                break;
            case 't':
                option |= FILE_TIM;
                break;
            case 'S':
                option |= FILE_SIZ;
                break;
            case 'h':
                option |= FILE_UND;
                break;
            case 'r':
                option |= FILE_REV;
                break;
            case 'R':
                option |= FILE_REC;
                break;
            default:
                err_exit("usage: a.out [-la] [directory...]");
                break;
            }
    if (argc > 0)
        while (argc-- > 0)
            show_dir(*argv++);    /*  处理目录列表  */
    else
        show_dir(".");    /*  默认情况下，处理当前目录  */
    exit(0);
}

/*  show_dir函数：遍历目录  */
void show_dir(char *pathname)
{
    DIR             *dp;
    struct dirent   *dirp;
    struct stat     statbuf[FILE_MAX];
    char            buf[FILE_MAX][FILENAME_MAX + 1];    /*  保存每个文件名  */
    char            path[FILE_MAX][PATH_MAX + 1];    /*  保存文件的路径  */
    int             filenum = 0;    /*  目录下的文件数  */
    struct winsize  size;
    int             width;     /*  输出宽度  */
    int             i;

    if ((dp = opendir(pathname)) == NULL)     /*  打开目录  */
        err_exit("%s", strerror(errno));
    while (filenum < FILE_MAX && (dirp = readdir(dp)) != NULL) {    /*  读取目录  */
        if (!(option & FILE_ALL))
            if (dirp->d_name[0] == '.')    /*  跳过隐藏文件  */
                continue;
        strcpy(buf[filenum++], dirp->d_name);
    }
    qsort(buf, filenum, sizeof(buf[0]), filecmp);    /*  对文件名进行排序  */ 

    if (ioctl(1, TIOCGWINSZ, &size) < 0)    /*  获取终端窗口的大小  */
        err_exit("%s", strerror(errno));
    width = get_width(size.ws_col, buf, filenum);

    for (i = 0; i < filenum; i++)
        if (option & FILE_LON) {
            strcpy(path[i], pathname);
            strcat(path[i], "/");
            strcat(path[i], buf[i]);
            if (lstat(path[i], &statbuf[i]) < 0)
                err_exit("%s", strerror(errno));
        } else
            break;

    if (option & FILE_TIM)     /*  按文件时间进行排序  */
        qsort(statbuf, filenum, sizeof(statbuf[0]), timecmp);
    else if (option & FILE_SIZ)    /*  按文件大小进行排序  */
        qsort(statbuf, filenum, sizeof(statbuf[0]), sizecmp);

    for (i = 0; i < filenum; i++)
        if (option & FILE_LON) {
            show_attribute(statbuf[i]);
            printf("%-s\n", buf[i]); 
        } else
            show_align(size.ws_col, width, buf[i]);
    if (!(option & FILE_LON))
        putchar('\n');
}

/*  get_width函数：计算出用于show_align函数的输出宽度  */
int get_width(int ws_col, char buf[][PATH_MAX + 1], int filenum)
{
    int maxlen;    /*  最大文件名长度  */
    int i;

    for ( maxlen = i = 0; i < filenum; i++) 
        if (strlen(buf[i]) > maxlen)
            maxlen = strlen(buf[i]);
    return maxlen;
}

/*  show_align函数：将目录下的所有文件按列对齐输出  */
void show_align(int ws_col, int width, char *pathname)
{
    static int row;

    if ((row += width) > ws_col) {
        putchar('\n');
        row = width + 1;    /*  文件间以空格隔开  */
    } else
        row += 1;
    printf("%-*s ", width, pathname);
}

/*  show_attribute函数：打印文件的详细属性信息  */
void show_attribute(struct stat statbuf)
{
    struct passwd   *usr;
    struct group    *grp;
    char            *timeptr;

    /*  打印文件的类型信息  */
    if (S_ISREG(statbuf.st_mode))
        printf("-");
    else if (S_ISDIR(statbuf.st_mode))
        printf("d");
    else if (S_ISLNK(statbuf.st_mode))
        printf("l");
    else if (S_ISCHR(statbuf.st_mode))
        printf("c");
    else if (S_ISBLK(statbuf.st_mode))
        printf("b");
    else if (S_ISFIFO(statbuf.st_mode))
        printf("f");
    else if (S_ISSOCK(statbuf.st_mode))
        printf("s");

    /*  打印文件的权限信息  */
    printf("%c", statbuf.st_mode & S_IRUSR ? 'r' : '-');
    printf("%c", statbuf.st_mode & S_IWUSR ? 'w' : '-');
    printf("%c", statbuf.st_mode & S_IXUSR ? 'x' : '-');
    printf("%c", statbuf.st_mode & S_IRGRP ? 'r' : '-');
    printf("%c", statbuf.st_mode & S_IWGRP ? 'w' : '-');
    printf("%c", statbuf.st_mode & S_IXGRP ? 'x' : '-');
    printf("%c", statbuf.st_mode & S_IROTH ? 'r' : '-');
    printf("%c", statbuf.st_mode & S_IWOTH ? 'w' : '-');
    printf("%c", statbuf.st_mode & S_IXOTH ? 'x' : '-');

    /*  打印文件的链接数  */
    printf(" %4d ", statbuf.st_nlink);

    /*  打印文件的所有者和用户组  */
    if ((usr= getpwuid(statbuf.st_uid)) == NULL)
        err_exit("%s", strerror(errno));
    if ((grp = getgrgid(statbuf.st_gid)) == NULL)
        err_exit("%s", strerror(errno));
    printf("%-10s ", usr->pw_name);
    printf("%-10s ", grp->gr_name);

    /*  打印文件的大小  */
    if (option & FILE_UND)
        fsize_trans(statbuf.st_size);
    else
        printf("%6lld ", statbuf.st_size);

    /*  打印文件的时间  */
    if ((timeptr = ctime(&statbuf.st_mtime)) == NULL)
        err_exit("%s", strerror(errno));
    if (timeptr[strlen(timeptr) - 1] == '\n')
        timeptr[strlen(timeptr) - 1] = '\0';
    printf("%s ", timeptr);
}

#define GB (1024 * 1024 * 1024)
#define MB (1024 * 1024)
#define KB (1024)

/*  size_trans函数：以人类易懂的方式打印文件大小  */
void fsize_trans(off_t size)
{
    if (size / GB)
        printf("%4.1lfG ", 1.0 * size / GB);
    else if (size / MB)
        printf("%4.1lfM ", 1.0 * size / MB);
    else if (size / KB)
        printf("%4.1lfK ", 1.0 * size / KB);
    else
        printf("%4lldB ", size);
}

/*  filecmp函数：比较文件名  */
int filecmp(const void *s, const void *t)
{
    return strcmp(s, t) * 
        ((option & FILE_REV) ? -1 : 1);
}

/*  timecmp函数：比较文件时间  */
int timecmp(const void *s, const void *t)
{
    return (((struct stat *)s)->st_mtime 
        - ((struct stat *)t)->st_mtime) 
        * ((option & FILE_REV) ? -1 : 1);
}

/*  sizecmp函数：比较文件大小  */
int sizecmp(const void *s, const void *t)
{
    return (((struct stat *)s)->st_size 
        - ((struct stat *)t)->st_size)
        * ((option & FILE_REV) ? -1 : 1);
}

/*  err_exit函数：打印出错信息，并终止程序运行  */
void err_exit(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
