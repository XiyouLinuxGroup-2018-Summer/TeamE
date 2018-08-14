#include "shell.h"

/*Print shell*/
void Print_shell(void){
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
/*show getinput for user*/
void show_history(){
	int i = 0;
	printf("Can save 100 history command.\n");
	for(i=0; i<H_len%100; i++){
		printf("%d : %s", i+1, history[i]);
	}
}
/*chagnge the work environ*/
void Do_Cd(){
    if(argument[1] == NULL){
        //printf("Please enter the pathname!\n");
		return;
    }else{
        if(chdir(argument[1]) == -1){
            printf("%d:No this Pathname!\n", __LINE__);
        }else{
           return;
        }
    }
}
/*do ls*/
void Do_Ls(){
    int pid;
	int status;
	pid = fork();
	if(pid < 0){
		printf("%d:fork failed in Do_Ls\n", __LINE__);
	}else if(pid == 0){
		execvp("/home/zhe/TeamE/Code/zhuyichen/ls/ls", argument);
	}else{
		wait(&status);
	}
}
void Hand_argu(char *arg, int *k){
    char *p = arg;
    char *q = arg;
    int number = 0;
    while (1){
        if(p[0] == '\n'){
            break;
        }
        if(p[0] == ' '){
            p++;
        }else{
            q = p;
            number = 0;
            while(q[0]!=' ' && q[0]!='\n'){
                number++;
                q++;
            }
            argument[*k] = (char *)malloc(number+1);
            strncpy(argument[*k],p,number+1);
            argument[*k][number] = 0;
            *k = *k + 1;
            p = q;
        }
    }
}
void Print(int k){
    int i;
    for(i=0; i<k; i++){
        printf("argument[%d]=%s\n", i+1, argument[i]);
    }
}
void Deal_argu(int k){
    int i;
    int flag = 0;
    char *arg[k+1];
    char *filename;
    char *argnext[k+1];
	int pid;
	int status;
	int fd;
    for(i=0; i<k; i++){
        arg[i] = argument[i];
    }
    arg[k] = NULL;
    for(i=0; i<k; i++){
        if(strncmp(arg[i], "&", 1) == 0){
            if(i == k-1){
                shell_value.Back = 1;
                arg[k-1] = NULL;
                break;
            }else{
                printf("%d:wrong argument.\n", __LINE__);
                return;
            }
        }
    }
    for(i=0; arg[i]!=NULL; i++){
        if(strcmp(arg[i], ">>") == 0){
            flag++;
			//printf("this is double dayu.\n");
            shell_value.Double_Dayu = 1;
        }
        if(strcmp(arg[i], ">") == 0){
            flag++;
			//printf("this is dayu.\n");
            shell_value.Dayu = 1;
        }
        if(strcmp(arg[i], "<") == 0){
            flag++;
			//printf("this is xiaoyu.\n");
            shell_value.Xiaoyu = 1;
        }
        if(strcmp(arg[i], "|") == 0){
            flag++;
			//printf("this is Or.\n");
            shell_value.Or = 1;
        }
    }
    if(flag > 1){
        printf("%d:argument wrong", __LINE__);
        return;
    }
    if(shell_value.Dayu || shell_value.Double_Dayu){
        for(i=0; arg[i]!=NULL; i++){
            if(strncmp(arg[i], ">>",2) == 0||
				strcmp(arg[i], ">") == 0){
                filename = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(shell_value.Xiaoyu){
        for(i=0; arg[i]!=NULL; i++){
            if(strcmp(arg[i], "<") == 0){
                filename = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(shell_value.Or){
        for(i=0; arg[i]!=NULL; i++){
            if(strcmp(arg[i], "|") == 0){
                arg[i] = NULL;
                int j;
                for(j=i+1; arg[j] != NULL; j++){
                    argnext[j-i-1] = arg[j];
                }
				argnext[j-i-1] = arg[j];
				break;
            }
        }
    }
	if(shell_value.Xiaoyu == 0 &&
		shell_value.Or == 0 &&
		shell_value.Double_Dayu == 0 &&
		shell_value.Dayu == 0 &&
		shell_value.Back == 0){
			if (strncmp(argument[0], "cd", 2) == 0){
				Do_Cd();
				return;
			}
			/*if(strncmp(argument[0], "ls",2) == 0){
				Do_Ls();
			}*/
			if(strncmp(argument[0], "history", 7) == 0){
				show_history();
				return;
			}
			pid = fork();
			if(pid == 0){
				execvp(arg[0], arg);
			}
            if(shell_value.Back){
                printf("[process id %d]\n",pid);
                return ;
            }
			wait(&status);
	}
	if(shell_value.Dayu){
		pid = fork();
		if(pid < 0){
			printf("%d:fork failed.\n", __LINE__);
		}
		if(pid == 0){
			fd = open(filename,O_RDWR|O_CREAT|O_TRUNC, 0777);
			dup2(fd,1);
			close(fd);
			if(strncmp(arg[0], "history", 7) == 0){
				show_history();
                exit(0);
			}
			execvp(arg[0], arg);
		}
        if(shell_value.Back){
            printf("[process id %d]\n",pid);
            return ;
        }
		wait(&status);
        return;
	}
	if(shell_value.Double_Dayu){
		pid = fork();
		if(pid < 0){
			printf("%d:fork failed.\n", __LINE__);
		}
		if(pid == 0){
			fd = open(filename,O_RDWR|O_CREAT|O_APPEND, 0777);
			dup2(fd,1);
			close(fd);
			//execvp(/*"/home/zhe/TeamE/Code/zhuyichen/ls/ls"*/argument[0], arg);
			if(strncmp(arg[0], "history", 7) == 0){
                show_history();
                exit(0);
			}
            execvp(/*"/home/zhe/TeamE/Code/zhuyichen/ls/ls"*/arg[0], arg);
		}
        if(shell_value.Back){
            printf("[process id %d]\n",pid);
            return ;
        }
		wait(&status);
	}
    /*Input */
	if(shell_value.Xiaoyu){
        pid = fork();
		if(pid < 0){
			printf("%d:fork failed.\n", __LINE__);
		}
		if(pid == 0){
			fd = open(filename,O_RDONLY);
            if(fd == -1){
                printf("No this file .\n");
                exit(0);
            }
			dup2(fd,0);
			close(fd);
			//execvp(/*"/home/zhe/TeamE/Code/zhuyichen/ls/ls"*/argument[0], arg);
			if(strncmp(arg[0], "history", 7) == 0){
                show_history();
                exit(0);
			}

            execvp(/*"/home/zhe/TeamE/Code/zhuyichen/ls/ls"*/arg[0], arg);
		}
        if(shell_value.Back){
            printf("[process id %d]\n",pid);
            return ;
        }
		wait(&status);
	}
    /*guan dao*/
	if(shell_value.Or){
        int i;
        //for(i=0; argnext[i]!=NULL; i++){
          //  printf("argnext is ;%s;\n", argnext[i]);
        //}
        pid = fork();
        if(pid == 0){
            int pid2;
            int status2;
            int fd2;
            pid2 = fork();
            if(pid2 < 0){
                printf("%d:fork2 failed.\n",__LINE__);
            }else if(pid2 == 0){
                fd2 = open("/home/zhe/Desktop/Process/shell/test", O_RDWR|O_CREAT|O_TRUNC, 0777);
                dup2(fd2,1);
                close(fd2);
                execvp(arg[0],arg);
                exit(0);
            }

            if(wait(&status2) == -1){
                printf("%d: wait child pid2 error.\n", __LINE__);
            }

            fd2 = open("/home/zhe/Desktop/Process/shell/test", O_RDONLY);
            dup2(fd2,0);
            close(fd2);
            execvp(argnext[0], argnext);
            exit(0);

            if(remove("/home/zhe/Desktop/Process/shell/test")){
                printf("%d: remove file test failed.\n", __LINE__);
                exit(0);
            }
        }

        if(shell_value.Back){
            printf("[process id %d]\n",pid);
            return;
        }
        wait(&status);
	}
}
void ha(int sig){
    Print_shell();
}
int main(void){

    char argu[100];
    while (1){
        int k = 0;
        signal(SIGINT, ha);
        memset(argu,0,100);
        memset(&shell_value,0,sizeof(shell));
        memset(argument,0,sizeof(argument));
        Print_shell();
        fgets(argu, 100, stdin);
        while(argu[0] == '\n'){
            Print_shell();
            fgets(argu, 100, stdin);
        }
        history[H_len%100] = (char *)malloc(strlen(argu));
		strcpy(history[(H_len++)%100], argu);
        Hand_argu(argu, &k);
		//Print(k);
        Deal_argu(k);
    }
    return 0;
}