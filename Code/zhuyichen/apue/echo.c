#include "apue.h"

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
    exit(0);
}
