#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <pwd.h>

#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define END "\x1b[0m"

typedef struct Shell{
    unsigned int Dayu:1;
    unsigned int Double_Dayu:1;
    unsigned int Xiaoyu:1;
    unsigned int Or:1;
    unsigned int Back:1;
}shell;

shell shell_value;

int H_len = 0;
char *argument[100];
char *history[100];

void Do_Cd();
void Print_shell(void);
void Hand_argu(char *arg,int *k);
void Do_Ls();
void show_history();
void Deal_argu(int k);
void Print(int k);