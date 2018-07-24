#include <unistd.h>
#include "apue.h"

#define MAXPATH 1000

int main(int argc, char **argv)
{
    char *ptr;

    if (argc != 2) 
        err_quit("usage: a.out [filename]");
    
    if ((ptr = malloc(MAXPATH)) == NULL)
        err_sys("alloc failed");
    if (getcwd(ptr, MAXPATH) == NULL)
        err_sys("getcwd failed");
    printf("%s\n", ptr);
    free(ptr);
    return 0;
}
