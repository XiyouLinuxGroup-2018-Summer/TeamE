#include <stdio.h>
#include <ctype.h>

#define MAXLINE 100

int main(void)
{
    char buf[MAXLINE];
    char *ptr;
    int state;
    enum {OUT, IN};

    state = OUT;
    while ((ptr = fgets(buf, MAXLINE - 1, stdin)) != NULL) {
        for ( ; *ptr != '\0'; ptr++) 
            if (isspace(*ptr))
                state = OUT;
            else if (state == OUT) {
                *ptr = toupper(*ptr);
                state = IN;
            }
        fputs(buf, stdout);
    }
    return 0;
}
