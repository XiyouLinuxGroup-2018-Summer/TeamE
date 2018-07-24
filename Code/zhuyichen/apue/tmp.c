#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "apue.h"

int main(int argc, char **argv)
{
    
    struct stat buf;
    char *ptr;    
    DIR             *dp;
    struct dirent   *dirp;

    dp = opendir(argv[1]);
    while ((dirp = readdir(dp)) != NULL) {
        if (lstat(dirp->d_name, &buf) < 0) {
            err_ret("lstat error");
            continue;
        }
        if (S_ISREG(buf.st_mode))
            ptr = "regular";
        else if (S_ISDIR(buf.st_mode))
            ptr = "directory";
        else if (S_ISCHR(buf.st_mode))
            ptr = "character special";
        else if (S_ISBLK(buf.st_mode))
            ptr = "block special";
        else if (S_ISLNK(buf.st_mode))
            ptr = "symbolic link";
        else if (S_ISFIFO(buf.st_mode))
            ptr = "fifo";
        else if (S_ISSOCK(buf.st_mode))
            ptr = "socket";
        else
            ptr = "** unknown mode **";
        printf("%s\n", ptr);
    }   
}
