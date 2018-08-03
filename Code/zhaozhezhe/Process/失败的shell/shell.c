#include "shell.h"

extern char **environ;
int H_len = 0;
char *history[100];
//打印当前的shell提示
void Print_SHELL(void){

	struct passwd *pwd;
	char buf[256];
	char hostname[256];
	getcwd(buf,256);
	gethostname(hostname,256);
	pwd = getpwuid(getuid());
	printf(GREEN "%s:" END, pwd->pw_name);
	printf(GREEN "%s" END, hostname);
	printf(BLUE ": %s " END, buf);
}
//打印字符串
void Echo_string(char *string){
	int i = 0;
	for(i=0; string[i]==' '; i++);
	printf("%s\n", string);
	//printf("%s", &string[i]);
}
//cd切换目录
void SHELL_CD(char *string){
	int j,k;
	for(j=0; string[j]==' '; j++);
	//string[j+strlen(&string[j])-1] = 0;
	//printf(":%s:\n", &string[j]);
	if(chdir(&string[j]) == -1){
		//chdir("..");
		//Print_SHELL();
		printf("This is wrong pathname %s\n", &string[j+k]);
		printf("No this pathname!");
		exit(1);
	}else{
		//printf("print shell!\n");
		Print_SHELL();
	}
}
void SHELL_LS(char *value){
	
	int pid;
	int status;
	pid = fork();
	if(pid < 0){
		printf("失败!\n");
	}else if(pid == 0){
		int i;
		int j;
		int k=0;
		char *argv[9]={0};

		argv[k] = (char *)malloc(2);
		strcpy(argv[k++], "ls");
		
		for(j=0; value[j]==' '; j++);

		for(i=0; i<strlen(&value[j]); i++){
			if(value[j+i] == ' '){
				value[j+i] = 0;
			}
		}
		while (strlen(&value[j]) >= 2){
			argv[k] = (char *)malloc(strlen(&value[j]));
			strcpy(argv[k],&value[j]);
			k++;
			j += strlen(&value[j])+1;
		}

		execvp("/home/zhe/TeamE/Code/zhuyichen/ls/ls", argv/*, environ*/);
	}else{
		wait(&status);
	}

}
void Show_history(){
	int i = 0;
	for(i=0; i<H_len%100; i++){
		printf("%d : %s\n", i+1, history[i]);
	}
}
void Handle_argu(char *string, int length){

	int i;
	for(i=0; i<length; i++){
		if(string[i] == ' '){
			continue;
		}else{
			if(strncmp(&string[i], "echo", 4) == 0){
				Echo_string(&string[i+5]);
			}
			if(strncmp(&string[i], "cd", 2) == 0){
				SHELL_CD(&string[i+2]);
			}
			if(strncmp(&string[i], "exit", 4) == 0
					||strncmp(&string[i],"logout", 5) ==0){
				printf("If you want to leave? [y/n]");
				char a = getchar();
				if(a == 'y'){
					exit(0);
				}else{
					Print_SHELL();	
				}
			}
			if(strncmp(&string[i], "history", 7) == 0){
				Show_history();
			}
			if(strncmp(&string[i], "ls", 2) == 0){
				SHELL_LS(&string[i+2]);
			}
		}
	}
}
//void Hand_signal(int signo){
//	Print_SHELL();
//}
int main(void){
	
	while(1){
		//signal(SIGINT, Hand_signal);
		int i = 0;
		shell s_value;
		memset(&s_value,0,sizeof(shell));
		char value_string[10000];
		memset(value_string, 0, 10000);
		Print_SHELL();
		fgets(value_string, 10000, stdin);
		value_string[strlen(value_string)-1] = 0;	
		history[H_len%100] = (char *)malloc(strlen(value_string));
		strcpy(history[H_len%100], value_string);
		H_len++;
		//printf("%d : %s", strlen(value_string), value_string);
		Handle_argu(value_string, strlen(value_string));
		printf("\r");
	}
}
