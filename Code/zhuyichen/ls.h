#ifndef _LS_H
#define _LS_H

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

#define FILE_ALL    0001     /*  -a  */
#define FILE_LON    0002     /*  -l  */
#define FILE_TIM    0004     /*  -t  */
#define FILE_SIZ    0010     /*  -s  */
#define FILE_UND    0020     /*  -h  */
#define FILE_REV    0040     /*  -r  */
#define FILE_REC    0100     /*  -r  */
#define FILE_NUM    0200     /*  -n  */
#define FILE_INO    0400     /*  -i  */

#define GB      (1024 * 1024 * 1024)
#define MB      (1024 * 1024)
#define KB      (1024)

#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define END         "\033[0m"

struct statf {          /*  每个文件的所有信息  */
    struct stat     statbuf;   
    char            buf[FILENAME_MAX + 1];     /*  保存文件名  */
    char            path[PATH_MAX + 1];        /*  保存文件路径  */
};

int (*cmp)(const void *, const void *);
int filecmp(const void *, const void *);
int timecmp(const void *, const void *);
int sizecmp(const void *, const void *);

void deal_argfile(int argc, char **argv);
void show_dir(char *pathname, int fn);
void show_single_dir(struct statf *statfp[], int fn);
int get_fn(char *pathname);
int get_width(int ws_col, struct statf *statfp[], int fn);
void show_align(int ws_col, int width, struct statf *staf);
void show_attribute(struct statf *staf);
void fsize_trans(off_t size);
void err_exit(char *fmt, ...);

#endif    /*  _LS_H  */
