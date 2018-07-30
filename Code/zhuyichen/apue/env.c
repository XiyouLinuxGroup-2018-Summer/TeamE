#include "apue.h"

int main(int argc, char **argv)
{
    int         i;
    char        **ptr;
    extern char **environ;

    for (i = 0; i < argc; i++)
        printf("aargv[%d]: %s\n", i, argv[i]);

    for (ptr = environ; *ptr != NULL; ptr++)
        printf("%s\n", *ptr);
    exit(0);
}
