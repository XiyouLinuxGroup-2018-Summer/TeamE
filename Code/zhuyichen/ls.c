#include "ls.h"

static int option = 0;       /*  设置标志位  */
/*
 * ls命令的简要实现，可用于处理[-ahilnRrSt]参数 
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
            case 'n':
                option |= FILE_NUM;
                break;
            case 'i':
                option |= FILE_INO;
                break;
            default:
                err_exit("usage: a.out [-ahilnRrSt] [directories or files...]");
                break;
            }
    if (argc > 0)
        deal_argfile(argc, argv);    /*  处理文件参数表  */
    else
        show_dir(".", get_fn("."));    /*  默认情况下，处理当前目录  */
    exit(0);
}

/*  deal_argfile函数：处理文件列表参数，并判断其是目录还是文件  */
void deal_argfile(int argc, char **argv)
{
    DIR             *dp;
    char            *dirbuf[argc];    /* 保存文件参数表中的目录名 */
    struct statf    *statfp[argc];    /* 保存文件参数表中的文件的详细信息 */
    int             dn, fn, i;

    dn = fn = 0;
    while (argc-- > 0)
        if ((dp = opendir(*argv)) == NULL) {   /*  可能是文件  */
            if ((statfp[fn] = malloc(sizeof(struct statf))) == NULL)
                err_exit("%s", strerror(errno));
            if (lstat(*argv, &statfp[fn]->statbuf) < 0)
                err_exit("%s", strerror(errno));
            strcpy(statfp[fn++]->buf, *argv++);    /*  这种情况下，statfp中的path[]将会被浪费  */
        } else {
            closedir(dp);   /*  是目录  */
            if ((dirbuf[dn++] = strdup(*argv++)) == NULL)
                err_exit("%s", strerror(errno));
        }

    if (fn > 0) {     /*  将文件参数表中的所有文件一起打印出来  */
        show_single_dir(statfp, fn);
        putchar('\n');
        for (i = 0; i < fn; i++)
            free(statfp[i]);
    }

    for (i = 0; i < dn; i++)    /*  处理目录列表  */
        show_dir(dirbuf[i], get_fn(dirbuf[i]));
}

/*  get_fn函数：获得pathname下的文件总数  */
int get_fn(char *pathname)
{
    DIR             *dp;
    struct dirent   *dirp;
    int             fn;

    fn = 0;
    if ((dp = opendir(pathname)) == NULL)     /*  打开目录  */
        err_exit("%s", strerror(errno));
    printf(MAGENTA"%-s:\n"END, pathname);
    while ((dirp = readdir(dp)) != NULL) {    /*  读取目录  */
        if (!(option & FILE_ALL))
            if (dirp->d_name[0] == '.')       /*  跳过隐藏文件  */
                continue;
        fn++;     /*  统计目录下的文件数  */
    }
    closedir(dp);
    return fn;
}

/*  show函数：列出目录下的文件列表  */
void show_dir(char *pathname, int fn)
{
    DIR             *dp;
    struct dirent   *dirp;
    struct statf    *statfp[fn];
    int             i;

    for (i = 0; i < fn; i++)    /*  为数组statfp分配内存  */
        if ((statfp[i] = malloc(sizeof(struct statf))) == NULL)
            err_exit("%s", strerror(errno));

    if ((dp = opendir(pathname)) == NULL)     
        err_exit("%s", strerror(errno));
    for (i = 0; (dirp = readdir(dp)) != NULL; i++) {   
        if (!(option & FILE_ALL))
            if (dirp->d_name[0] == '.') {
                i--;
                continue;
            }      /*  保存目录下每个文件的详细信息  */ 
        strcpy(statfp[i]->buf, dirp->d_name);
        strcpy(statfp[i]->path, pathname);
        strcat(statfp[i]->path, "/");
        strcat(statfp[i]->path, statfp[i]->buf);
        if (lstat(statfp[i]->path, &statfp[i]->statbuf) < 0)
            err_exit("%s", strerror(errno));
    }
    closedir(dp);

    show_single_dir(statfp, fn);    /*  打印单个目录下的所有内容  */

    if (option & FILE_REC)    /*  递归遍历目录  */
        for (i = 0; i < fn; i++)
            if (strcmp(statfp[i]->buf, ".") != 0
            && strcmp(statfp[i]->buf, "..") != 0
            && S_ISDIR(statfp[i]->statbuf.st_mode))
                show_dir(statfp[i]->path, get_fn(statfp[i]->path));

    for (i = 0; i < fn; i++)    /*  释放数组statfp的内存  */
        free(statfp[i]);
}

/*  show_single_dir函数：输出单个目录下的内容  */
void show_single_dir(struct statf *statfp[], int fn)
{
    struct winsize  size;     /*  终端窗口的大小  */
    int             width;    /*  输出宽度  */
    int             i;

    if (isatty(1)) {     /*  输出至终端  */
        if (ioctl(1, TIOCGWINSZ, &size) < 0)    /*  获取终端窗口的大小  */ 
            err_exit("%s", strerror(errno));
        width = get_width(size.ws_col, statfp, fn);   /*  得到输出宽度  */
    } else
        width = 0;    /*  输出重定向到了一个文件  */

    cmp = ((option & FILE_SIZ) ? sizecmp : ((option & FILE_TIM) ? timecmp : filecmp));
    qsort(statfp, fn, sizeof(statfp[0]), cmp);     /*  根据可选参数对文件进行排序  */

    for (i = 0; i < fn; i++)     /*  打印文件列表  */
        if (option & FILE_LON)
            show_attribute(statfp[i]);
        else
            show_align(size.ws_col, width, statfp[i]);
    if (!(option & FILE_LON))
        putchar('\n');

}

/*  get_width函数：计算出用于show_align函数的输出宽度  */
int get_width(int ws_col, struct statf *statfp[], int fn)
{
    int i;
    int maxlen;    /*  最大文件名长度  */

    for (maxlen = i = 0; i < fn; i++)    /*  获取最大文件名的长度  */
        if (strlen(statfp[i]->buf) > maxlen)
            maxlen = strlen(statfp[i]->buf);
    return maxlen;
}

/*
 * 如果输出到终端，则对齐输出
 * 如果输出重定向到了一个文件，则一行输出一个
 */

/*  show_align函数：将目录下的所有文件按列对齐输出  */
void show_align(int ws_col, int width, struct statf *staf)
{
    static int row;

    if (width) {
        if ((row += width) > ws_col) {
            putchar('\n');
            row = width + 1;    /*  文件间以空格隔开  */
        } else
            row += 1;
    }

    /* printf("%-*s ", width, staf->buf); */
    /*  根据不同文件类型显示不同颜色  */
    if (S_ISREG(staf->statbuf.st_mode))
        printf(CYAN"%-*s "END, width, staf->buf);
    else if (S_ISDIR(staf->statbuf.st_mode))
        printf(MAGENTA"%-*s "END, width, staf->buf);
    else if (S_ISLNK(staf->statbuf.st_mode))
        printf(YELLOW"%-*s "END, width, staf->buf);
    else if (S_ISCHR(staf->statbuf.st_mode))
        printf(RED"%-*s "END, width, staf->buf);
    else if (S_ISBLK(staf->statbuf.st_mode))
        printf(RED"%-*s "END, width, staf->buf);
    else if (S_ISFIFO(staf->statbuf.st_mode))
        printf(CYAN"%-*s "END, width, staf->buf);
    else if (S_ISSOCK(staf->statbuf.st_mode))
        printf(MAGENTA"%-*s "END, width, staf->buf);
    if(!width)
        putchar('\n');
}

/*  show_attribute函数：打印文件的详细属性信息  */
void show_attribute(struct statf *staf)
{
    struct passwd   *usr;
    struct group    *grp;
    char            *timeptr;

    /*  打印文件的inode编号  */
    if (option & FILE_INO)
        printf("%8llu ", staf->statbuf.st_ino);

    /*  打印文件的类型信息  */
    if (S_ISREG(staf->statbuf.st_mode))
        printf("-");
    else if (S_ISDIR(staf->statbuf.st_mode))
        printf("d");
    else if (S_ISLNK(staf->statbuf.st_mode))
        printf("l");
    else if (S_ISCHR(staf->statbuf.st_mode))
        printf("c");
    else if (S_ISBLK(staf->statbuf.st_mode))
        printf("b");
    else if (S_ISFIFO(staf->statbuf.st_mode))
        printf("f");
    else if (S_ISSOCK(staf->statbuf.st_mode))
        printf("s");

    /*  打印文件的权限信息  */
    printf("%c", staf->statbuf.st_mode & S_IRUSR ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWUSR ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXUSR ? 'x' : '-');
    printf("%c", staf->statbuf.st_mode & S_IRGRP ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWGRP ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXGRP ? 'x' : '-');
    printf("%c", staf->statbuf.st_mode & S_IROTH ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWOTH ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXOTH ? 'x' : '-');

    /*  打印文件的链接数  */
    printf(" %4d ", staf->statbuf.st_nlink);

    /*  打印文件的所有者和用户组  */
    if (option & FILE_NUM) {
        printf("%6d ", staf->statbuf.st_uid);
        printf("%6d ", staf->statbuf.st_gid);
    } else {
        if ((usr = getpwuid(staf->statbuf.st_uid)) == NULL)
            err_exit("%s", strerror(errno));
        if ((grp = getgrgid(staf->statbuf.st_gid)) == NULL)
            err_exit("%s", strerror(errno));
        printf("%-10s ", usr->pw_name);
        printf("%-10s ", grp->gr_name);
    }

    /*  打印文件的大小  */
    if (option & FILE_UND)
        fsize_trans(staf->statbuf.st_size);
    else
        printf("%8lld ", staf->statbuf.st_size);

    /*  打印文件的时间  */
    if ((timeptr = ctime(&staf->statbuf.st_mtime)) == NULL)
        err_exit("%s", strerror(errno));
    if (timeptr[strlen(timeptr) - 1] == '\n')
        timeptr[strlen(timeptr) - 1] = '\0';
    printf("%s ", timeptr);

    /*  根据不同文件类型显示不同颜色  */
    if (S_ISREG(staf->statbuf.st_mode))
        printf(CYAN"%-s\n"END, staf->buf);
    else if (S_ISDIR(staf->statbuf.st_mode))
        printf(MAGENTA"%-s\n"END, staf->buf);
    else if (S_ISLNK(staf->statbuf.st_mode))
        printf(YELLOW"%-s\n"END, staf->buf);
    else if (S_ISCHR(staf->statbuf.st_mode))
        printf(RED"%-s\n"END, staf->buf);
    else if (S_ISBLK(staf->statbuf.st_mode))
        printf(RED"%-s\n"END, staf->buf);
    else if (S_ISFIFO(staf->statbuf.st_mode))
        printf(CYAN"%-s\n"END, staf->buf);
    else if (S_ISSOCK(staf->statbuf.st_mode))
        printf(MAGENTA"%-s\n"END, staf->buf);
}

/*  fsize_trans函数：以人类易懂的方式打印文件大小  */
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
    return strcmp((*(struct statf **)s)->buf
        , (*(struct statf **)t)->buf) 
        * ((option & FILE_REV) ? -1 : 1);
}

/*  timecmp函数：比较文件时间  */
int timecmp(const void *s, const void *t)
{
    return ((*(struct statf **)s)->statbuf.st_mtime 
        - (*(struct statf **)t)->statbuf.st_mtime) 
        * ((option & FILE_REV) ? -1 : 1);
}

/*  sizecmp函数：比较文件大小  */
int sizecmp(const void *s, const void *t)
{
    return ((*(struct statf **)s)->statbuf.st_size 
        - (*(struct statf **)t)->statbuf.st_size)
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
