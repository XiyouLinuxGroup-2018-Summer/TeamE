#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLINE 100

int main(void)
{
    int     n;
    char    buf[MAXLINE];
    char    *ptr;

    scanf("%d", &n);
    getchar();
    while (n-- > 0) {
        if ((ptr = fgets(buf, MAXLINE - 1, stdin)) == NULL)
            return 1;
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        if (!isalpha(*ptr) && *ptr != '_') {
            printf("no\n");
            continue;
        }
        while (*++ptr)
            if (!isalnum(*ptr) && *ptr != '_')
                break;
        if (*ptr)
            printf("no\n");
        else
            printf("yes\n");
    }
    return 0;
}
