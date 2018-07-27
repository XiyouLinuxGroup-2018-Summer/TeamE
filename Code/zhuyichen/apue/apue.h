#ifndef _APUE_H
#define _APUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>

#define MAXLINE 4096

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

char *path_alloc(size_t *);

void err_cont(int error, const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_exit(int error, const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);
void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);

#endif   /*  _APUE_H  */
