#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef struct file {
    int     fid;
    char    filename[128];
    int     mode;
    int     size;
    char    data[256];
    struct file *next;
} myfile;

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

myfile * get_file(char *pathname)
{
    DIR             *dp;
    struct dirent   *dirp;
    struct stat     buf;
    char            path[PATH_MAX + 1];
    myfile          *head, *p, *q;
    int             fd;
    int             ino = 1;

    if ((q = head = malloc(sizeof(myfile))) == NULL)
        err_exit("alloc failed");   /*  头节点中无数据  */
    
    if ((dp = opendir(pathname)) == NULL)
        err_exit("%s", strerror(errno));
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0
            || strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(path, pathname);
        strcat(path, "/");
        strcat(path, dirp->d_name);
        if (lstat(path, &buf) < 0)
            err_exit("lstat error");
        if ((p = malloc(sizeof(myfile))) == NULL)
            err_exit("alloc failed");
        head->fid = ino++;
        strcpy(head->filename, dirp->d_name);
        head->mode = buf.st_mode;
        head->size = buf.st_size;
        if ((fd = open(dirp->d_name, O_RDONLY)) < 0)
            ;
            /* err_exit("can't open %s", dirp->d_name); */
        if (read(fd, head->data, 256) < 0)
            ;
            /* err_exit("%s", strerror(errno)); */
        q->next = p;
        p->next = NULL;
        q = p;
    }
    return head;
}

void print_data(myfile *head)
{
    int fd;

    if (head == NULL)
        return;
    if ((fd = creat("./allfiles.dat", 0777)) < 0)
        err_exit("creat error");
    if ((fd = open("./allfiles.dat", O_WRONLY | O_APPEND)) < 0)
        err_exit("open error");
    
    head = head->next;
    while (head != NULL) {
        if (write(fd, &head->fid, 1) != 1)
            err_exit("write error");
        if (write(fd, head->filename, sizeof(head->filename)) != sizeof(head->filename))
            err_exit("write error");
        if (write(fd, &head->mode, 1) != 1)
            err_exit("write error");
        if (write(fd, &head->size, 1) != 1)
            err_exit("write error");
        if (write(fd, head->data, sizeof(head->data)) != sizeof(head->data))
            err_exit("write error");
        if (write(fd, "\n", 1) != 1)
            err_exit("write error");
        head = head->next;
    }
}

void change_data(myfile *head)
{
    int fd;
    int c, nl = 0;

    if ((fd = open("./allfiles.dat", O_RDONLY) < 0))
        err_exit("open error");
    while (read(fd, &c, 1) < 0) {
        if (c == '\n')
            nl++;
        if (nl == 6)
            ;
    }
}
    
int main(void)
{
    myfile *head = NULL;

    head = get_file("/usr/lib");    
    print_data(head);
    exit(0);
}
