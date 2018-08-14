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
typedef struct SHELL{
	int SHELL_LS:1;//ls命令
	int SHELL_IN:1;//写入并覆盖文件。
	int SEHELL_DOUBLE_IN:1;//追加文件尾部。
	int SHELL_OUT:1;//
	int SHELL_DOUBLE_OUT:1;
	int SHELL_BACK:1;//后台运行
	int SHELL_WC:1;//文件的行数，文件的单词数，文件的字符数
	int SHELL_CHMOD:1;//改变文件权限
	int SHELL_RM:1;//删除文件
	int SHELL_FILE:1;//得到文件类型
	int SHELL_ECHO:1;//回显字符串
	int SHELL_FIND:1;//找文件，打印文件路径

}shell;

void Print_SHELL(void);
void Handle_argu(char *argu, int length);
void Do_argu(shell *SHE);
void Chmod();
void Rm_file();
void Get_Filetype();
void Echo_string();
void Find_file();
void Back_pro();
void IN_OR_DOUBLEIN();
void SHELL_LS(char *value);
void SHELL_CD(char *string);
