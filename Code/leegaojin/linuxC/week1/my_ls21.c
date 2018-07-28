/*************************************************************************
	> File Name: my_ls2.c
	> Author:
	> Mail:
	> Created Time: 2018年07月26日 星期四 14时40分12秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
//#include<Linux/limits.h>
#include<dirent.h>
#include<grp.h>
#include<errno.h>
#include<pwd.h>
#include<unistd.h>


#define MAXROWLEN   80   //一行显示的最多字符数
#define flag_N 0
#define flag_A 1
#define flag_L 2
#define flag_R 4

int    f_leave_len = MAXROWLEN;
int    f_maxlen;


void my_err(const char *err_string,int line)
{
  fprintf(stderr, "line:%d ",line);
  perror(err_string);
  exit(1);
}

void display_attribute(struct stat buf,char* name)
{

  char    buf_time[32];
  struct passwd *psd;
  struct group  *grp;

  //获取并打印文件类型
  if(S_ISLNK(buf.st_mode)){
    printf("l");
  }else if (S_ISREG(buf.st_mode)) {
    printf("-");
  }else if (S_ISDIR(buf.st_mode)) {
    printf("d");
  } else if (S_ISCHR(buf.st_mode)) {
    printf("c");
  } else if (S_ISBLK(buf.st_mode)) {
    printf("b");
  } else if (S_ISFIFO(buf.st_mode)) {
    printf("f");
  } else if (S_ISSOCK(buf.st_mode)) {
    printf("a");
  }

  //获取并打印文件所有者的权限
  if (buf.st_mode & S_IRUSR) {
    printf("r");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IWUSR) {
    printf("w");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IXUSR) {
    printf("x");
  } else {
    printf("-");
  }

  //获取并打印文件所有者同组的用户对该文件的操作权限
  if (buf.st_mode & S_IRGRP) {
    printf("r");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IWGRP) {
    printf("w");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IXGRP) {
    printf("x");
  } else {
    printf("-");
  }

  //获取并打印其他用户对该文件的操作权
  if (buf.st_mode & S_IROTH) {
    printf("r");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IWOTH) {
    printf("w");
  } else {
    printf("-");
  }
  if (buf.st_mode & S_IXOTH) {
    printf("x");
  } else {
    printf("-");
  }

  //根据uid与gid获取文件所有者的用户名与组名
  psd = getpwuid(buf.st_uid);
  grp = getgrgid(buf.st_gid);
  printf("%4d",buf.st_nlink);
  printf(" ");
  printf("%-8s",psd->pw_name);
  printf("%-8s",grp->gr_name);

  printf("%6d",buf.st_size);
  strcpy(buf_time,ctime(&buf.st_mtime));
  buf_time[strlen(buf_time)-1] = '\0'; //去掉换行符
  printf(" %s",buf_time);
  printf(" %s",name);
  printf("\n");
}
void display_single(char *name)
{
   int i,len;

   if(f_leave_len<f_maxlen){
     printf("\n");
     f_leave_len = 80;
   }

   len = strlen(name);
   len = f_maxlen-len;
   printf("%-s",name);

   for(i=0;i<=len;i++)
   {
      printf(" ");
   }
   f_leave_len-=(f_maxlen+2);

}
void display_file(int flag,char *name)
{
  struct stat buf;
  DIR     *dir;
  struct dirent     *ptr;


  switch (flag) {
    case flag_N:
     if(name[0]!='.'){
       display_single(name);
    //   printf("\n");
     }
        break;
    case 1://-a
       display_single(name);
        break;
    case 2://-l
    if(name[0]!='.'){
      display_attribute(buf,name);
    }
        break;
    case 3://-al
      display_attribute(buf,name);
        break;
    default:
        break;
  }

}
void display_dir(int flag,char*name)
{
    DIR * dir;
    struct dirent  *ptr;
    int     i,count = 0;
    char   filenames[256][10000];
    //char    *filenames
    char name_dir[10000];
    if(chdir(name)<0)
    {
      my_err("chdir",__LINE__);
    }
    if(getcwd(name_dir,512)<0){
      my_err("getcwd",__LINE__);
    }
    printf("%s:\n",name_dir);

    dir = opendir(name);
    if(dir==NULL){
      my_err("opendir",__LINE__);
    }
    while((ptr = readdir(dir))!=NULL){
      if(f_maxlen<strlen(ptr->d_name))
             f_maxlen = strlen(ptr->d_name);
        count++;
    }
    closedir(dir);
    //动态数组
    //filenames=(char**)malloc(count*sizeof(char));
    //for(i=0;i<count;i++)
    //filenames[i]=(char*)malloc(f_maxlen*sizeof(char));

    int j,len=strlen(name);
    dir = opendir(name);
    for(i=0;i<count;i++){
        ptr = readdir(dir);
        if(ptr == NULL){
          my_err("readdir",__LINE__);
        }
        strcat(filenames[i],ptr->d_name);
        filenames[i][strlen(ptr->d_name)] = '\0';
    }
    for(i=0;i<count;i++)
       display_file(flag,filenames[i]);
      printf("\n");
  //  if(flag   )//参数中有R
  //  {
  //
    //}
  //     closedir(dir);


}

int main(int argc,char**argv)
{
    int flag_param = flag_N;
    int len,i,k,j=0;
    char param[10];
    char name[1000];
    struct stat   buf;
    int num=0;
    for(i=0;i<argc;i++)
    {
      if(argv[i][0]=='-')
        {
          for(k=1;k<strlen(argv[i]);k++)
          {
            param[j]=argv[i][k];
            j++;
          }
          num++;
        }
    }

    //支持参数-a -l -R
    for(i=0;i<j;i++)
    {
      if(param[i]=='a'){
        flag_param |= flag_A;
        continue;
      }
      else if(param[i]=='l'){
        flag_param |= flag_L;
        continue;
      }
      else if(param[i]=='R'){
        flag_param |= flag_R;
        continue;
      }else{
      printf("my_ls:invalid option -%c\n",param[i]);
      }
    }
    param[j]='\0';

if((num+1)==argc){
  strcpy(name,"./");
  name[2]='\0';
  display_dir(flag_param,name);
  //显示当前目录
  return 0;
}

i=1;
do{
  if(argv[i][0]=='-'){
    i++;
    continue;
  }else{
    strcpy(name,argv[i]);

    if(stat(name,&buf)==-1)
     my_err("stat",__LINE__);

    if(S_ISDIR(buf.st_mode) ){

      if(name[strlen(argv[i])-1]!='/'){
        name[strlen(argv[i])]!='/';
        name[strlen(argv[i])+1]='\0';
      }else{
        name[strlen(argv[i])] = '\0';

        display_dir(flag_param,name);
        i++;
        //读取目录 函数调用
      }
    }else{
        display_file(flag_param,name);
        i++;
      //读取文件 函数调用
    }
  }

}while(i<argc);


  //  display(flag_param,argv[2]);


    return 0;



}
