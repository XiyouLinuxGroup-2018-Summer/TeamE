#include "apue.h"
#include <errno.h>

int my_system(char *cmdstring);

int main(int argc, char **argv)
{
    while (*argv != NULL)
        my_system(*argv++);
    exit(0);
}

int my_system(char *cmdstring)
{
    pid_t   pid;
    int     status;

    if (cmdstring == NULL)
        return 1;

    if ((pid = fork()) < 0)
        status = -1;
    else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    } else {
        while (waitpid(pid, &status, 0) < 0)
            if (errno != EINTR) {
                status = -1;
                break;
            } 
    }
    return status;
}
