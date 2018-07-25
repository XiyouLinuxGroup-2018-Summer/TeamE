#include <stdio.h>
#include <string.h>

#define MAXLINE 1000   

void palindrome(char *s)
{
    int i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
        if (s[i] != s[j])
            break;
    if (i < j)
        printf("no\n");
    else
        printf("yes\n");
    
}

int main(void)
{
    int     n;
    char    buf[MAXLINE];

    scanf("%d", &n);
    getchar();    /*  跳过n后的\n  */
    while (n-- > 0) {
        if (fgets(buf, MAXLINE - 1, stdin) == NULL)
            return 1;
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        palindrome(buf);
    }
    return 0;
}
