/*************************************************************************
	> File Name: my_shell.c
	> Author:
	> Mail:
	> Created Time: 2018年07月30日 星期一 16时55分53秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

#define normal            0   //一般的命令
#define out_redirect      1   //输出重定向
#define in_redirect       2   //输入重定向
#define have_pipe         3   //命令中有管道
#define out_out_redirect  4

void print_prompt();                          //打印提示符
void get_input(char*);                        //得到输入的命令
void explain_input(char*,int *,char (*)[256]); //    //对输入的命令进行解析
void do_cmd(int,char (*)[256]); //                 //执行命令
int find_command(char *);                    //查找命令中的可执行程序

void my_err(const char* err_string,int line)
{
  fprintf(stderr,"line:%d ",line);
  perror(err_string);
  exit(1);
}
int main(int argc,char* argv[])
{
  signal(SIGINT,SIG_IGN);
  signal(SIGTSTP,SIG_IGN);
  int i;
  int argcount = 0;
  char arglist[100][256];
  char **arg=NULL;
  char *buf = NULL;
  //buf = (char*)malloc(256);
  //if(buf==NULL){
  //  perror("malloc failed");
  //  exit(-1);
  //}

  while(1){
    //将buf所指的空间清零
    //memset(buf,0,256);
    print_prompt();
    buf = readline(" ");
    add_history(buf);
    strcat(buf,"\n");
    //get_input(buf);
    //若输入的命令为exit或logout则退出本程序
    if(strcmp(buf,"exit\n")==0||strcmp(buf,"logout\n")==0)
        break;
    for(i=0;i<100;i++)
    {
      arglist[i][0]='\0';
    }
    argcount = 0;
    explain_input(buf,&argcount,arglist);
    do_cmd(argcount,arglist);
  }

  if(buf!=NULL){
    free(buf);
    buf = NULL;
  }
  exit(0);

}

void print_prompt()
{
  char buf[512];
  char * menu=NULL;

//  char *p = readline("myshell:");
//  add_history(p);

  if(getcwd(buf,512)<0){
    my_err("getcwd",__LINE__);
  }
   //获得主目录　用户名等
  //printf("%s\n",buf);
  printf("\033[1;35;40mlee@lee-PC:\033[0m");//更改输出格式字体颜色　（信号）（历史记录）
  if((menu=strstr(buf,"/home/lee"))!=NULL){
  printf("~");
  printf("\033[1;36;40m%s\033[0m$",buf+9);
}else{
  printf("\033[1;36;40m%s\033[0m$",buf);
  }
}

//获取用户输入
/*void get_input(char*buf)
{
  int len=0;
  int ch;
  char*prompt[100];
  char*input=NULL;

  //ch = getchar();
  //while (len<256&&ch!='\n'){
  //  buf[len++]=ch;
  //  ch = getchar();
  //}
  //if(len==256){
  //  printf("command is too long\n");
  //  exit(-1); //输入的命令过长则退出程序
  //}

  //buf[len] = '\n';
  //len++;
  //buf[len]='\0';
  buf = readline(" ");
  //add_history(buf);
  strcat(buf,"\n");
}
*/
//解析buf中的命令，将结果存入arglist中，命令以回车符号\n结束
//如输入的命令为"ls -l /tmp",则arglist[0]、arglist[1]、arglist[2]分别为ls
//-l 和　/tmp
void explain_input(char*buf,int *argcount,char arglist[100][256])
{
  char* p=buf;
  char* q=buf;
  int number = 0;
  while(1){
    if(p[0]=='\n')
      break;
    if(p[0]==' ')
     p++;
    else{
      q = p;
      number = 0;
      while((q[0]!=' ')&&(q[0]!='\n')){
        number++;
        q++;
      }
      strncpy(arglist[*argcount],p,number+1);
      arglist[*argcount][number] = '\0';
      *argcount = *argcount + 1;
      p = q;
    }
  }
}
void my_cd (char*file)
{
  struct stat buf;
  char name[512];
//  chdir(file);
//if(getcwd(name,512)<0){
//    my_err("getcwd",__LINE__);
  if(lstat(file,&buf)==-1)
  {
    perror("lstat");
    return;
  }
  if(S_ISDIR(buf.st_mode))//判断是否为目录，不是则输出不是后退出
  {
     if(chdir(file)<0)
     {
       my_err("chdir",__LINE__);
       return;
     }
  }else{
    printf("%s:not a directory\n",file);
    return;
  }
  //切换目录
  //打印信息

  //return;
}
void do_cmd(int argcount,char arglist[100][256])
{
  int     flag=0;
  int     how = 0;                //用于指示命令中是否含有>,<,l
  int     background = 0;         //标识命令中是否有后台运行标识符&
  int     status;
  int     i;
  int     fd;
  char*   arg[argcount+1];
  char*   argnext[argcount+1];
  char*   file;
  char    lastenter[512];
  pid_t   pid;
  //将命令取出
  for(i=0;i<argcount;i++)
  {
    arg[i]=(char*)arglist[i];
  }
  arg[argcount] = NULL;
  //内置cd
  if(argcount>0&&(strcmp(arg[0],"cd")==0))
  {
    //if(strchr(arg[1],'~')!=NULL)
    //{
    //  my_cd(strcat("/home/lee",arg[1]+1));
    //  return;
    //}
    my_cd(arg[1]);
    return;
  }
  //查看命令行是否有后台运行符
  for(i=0;i<argcount;i++)
  {
    if(strncmp(arg[i],"&",1)==0){
      if(i==argcount-1){
        background = 1;
        arg[argcount-1]=NULL;
        break;
      }
      else{
        printf("wrong command\n");
        return;
      }
    }
  }

  for(i=0;arg[i]!=NULL;i++)
  {
    if(strcmp(arg[i],">")==0){
      flag++;
      how = out_redirect;
      if((arg[i+1])==NULL)
        flag++;
    }
    if(strcmp(arg[i],">>")==0){
      flag++;
      how = out_out_redirect;
      if((arg[i+1])==NULL)
        flag++;
    }
    if(strcmp(arg[i],"<")==0){
      flag++;
      how = in_redirect;
      if(i==0)
        flag++;
    }
    if(strcmp(arg[i],"|")==0){
      flag++;
      how = have_pipe;
      if(arg[i+1]==NULL)
      flag++;
      if(i==0)
      flag++;
    }
  }

  //flag大于１，说明命令中含有多个>,<,|等符号，本程序不支持
  //或者格式不对
  if(flag>1){
    printf("wrong command\n");
    return;
  }

  if(how == out_redirect){ //命令只含有一个重定向符号
    for(i=0;arg[i]!=NULL;i++){
      if(strcmp(arg[i],">")==0){
        file = arg[i+1];
        arg[i]=NULL;
      }
    }
  }

  if(how == in_redirect){//命令只含一个重定向符号<
    for(i=0;arg[i]!=NULL;i++){
      if(strcmp(arg[i],"<")==0){
        file = arg[i+1];
        arg[i] = NULL;
      }
    }
  }

  if(how == have_pipe){//只含有一个管道符号，把管道后面的部分存入argnext中
    //后面部分为一个可执行的shell命令
    for(i=0;arg[i]!=NULL;i++){
      if(strcmp(arg[i],"|")==0){
        arg[i]=NULL;
        int j;
        for(j=i+1;arg[j]!=NULL;j++){
          argnext[j-i-1]=arg[j];
        }
        argnext[j-i-1]=arg[j];
        break;
      }
    }
  }

  if((pid=fork())<0){
    printf("fork error\n");
    return;
  }
  switch (how) {
    case 0: //pid为０说明是子进程，在子进程中执行输入的命令
    //输入的命令不含<、>、和|
    if(pid == 0){
      if(!(find_command(arg[0]))){
        printf("%s:command not found\n",arg[0]);
        exit(0);
      }
      execvp(arg[0],arg);
      exit(0);
    }
    break;

    case 1: //输入命令中含有重定向符>
    if(pid==0){
      if(!(find_command(arg[0]))){
        printf("%s :command not found\n",arg[0]);
        exit(0);
      }
      fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
      dup2(fd,1);
      execvp(arg[0],arg);
      exit(0);
    }
    break;
    case 2:
    //含有重定向符<
    if(pid == 0){
      if(!(find_command(arg[0]))){
        printf("%s :command not found\n",arg[0]);
        exit(0);
      }
      fd=open(file,O_RDONLY);
      dup2(fd,0);
      execvp(arg[0],arg);
      exit(0);
    }
    break;
    case 3:
    //含有管道符|
    if(pid==0){
      int pid2;
      int status2;
      int fd2;

      if((pid2=fork())<0){
        printf("fork2 error\n");
        return;
      }
      else if(pid2==0){
        if(!(find_command(arg[0]))){
          printf("%s :command not found\n",arg[0]);
          exit(0);
        }
        fd2 = open("/tmp/youdonotknowfile",O_WRONLY|O_TRUNC|O_CREAT,0644);
        dup2(fd2,1);
        execvp(arg[0],arg);
        exit(0);
      }
      if(waitpid(pid2,&status,0)==-1)
      printf("wait for child process error\n");

      if(!(find_command(argnext[0]))){
        printf("%s :command not found\n",argnext[0]);
        exit(0);
      }
      fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
      dup2(fd2,0);
      execvp(argnext[0],argnext);

      if(remove("/tmp/youdonotknowfile"))
      printf("remove error\n");
      exit(0);
    }
    break;
    case 4:
    if(pid==0){
      if(!(find_command(arg[0]))){
        printf("%s :command not found\n",arg[0]);
        exit(0);
      }
      fd = open(file,O_RDWR|O_CREAT|O_APPEND,0644);
      dup2(fd,1);
      execvp(arg[0],arg);
      exit(0);
    }
    break;
    default:
    break;
  }

  //若命令中有&，表示后台执行，父进程直接返回不等待子进程结束
  if(background ==1){
    printf("[process id %d]\n",pid);
    return;
  }
  //父进程等待子进程结束
  if(waitpid(pid,&status,0)==-1)
  printf("wait for child process error\n");
}

//查找命令中的可执行程序
int find_command(char*command)
{
  DIR* dp;
  struct dirent* dirp;
  char* path[] = {"./","/bin","/usr/bin",NULL};

  //是当前目录下的程序可以运行，如命令./fork可以被正确的解释和运行
  if(strncmp(command,"./",2)==0)
  command=command+2;

  //分别在当前目录/bin和/usr/bin 目录查找要执行的程序
  int i=0;
  while(path[i]!=NULL){
    if((dp=opendir(path[i]))==NULL)
    printf("Can not open /bin \n");
    while((dirp = readdir(dp))!=NULL){
      if(strcmp(dirp->d_name,command)==0){
        closedir(dp);
        return 1;
      }
    }
    closedir(dp);
    i++;
  }
  return 0;
}
