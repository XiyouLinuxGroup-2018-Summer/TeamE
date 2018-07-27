#include "apue.h"
#include <setjmp.h>

#define TOK_ADD 5

void do_line(char *);
void cmd_add(void);
int get_token(void);

jmp_buf jmpbuffer;
char *tok_ptr;

int main(void)
{
    char line[BUFSIZ];

    if (setjmp(jmpbuffer) != 0)
        printf("%d\n", setjmp(jmpbuffer));
    while (fgets(line, BUFSIZ - 1, stdin) != NULL)
        do_line(line);
}

void do_line(char *ptr)
{
    int cmd;

    tok_ptr = ptr;
    while ((cmd = get_token()) > 0)
        switch (cmd) {
        case TOK_ADD:
            cmd_add();
            break;
        }
}

void cmd_add(void)
{
    int token;

    token = get_token();
    if (token < 0)
        longjmp(jmpbuffer, 1);
}

int get_token(void)
{
    if (1 > 0)
        longjmp(jmpbuffer, 2);
}
