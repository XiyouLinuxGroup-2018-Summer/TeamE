/*************************************************************************
	> File Name: my_ls.c
	> Author:
	> Mail:
	> Created Time: 2018年07月25日 星期三 21时12分51秒
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

#define  PARAM_NONE  0   //无参数
#define  PARAM_A     1
#define  PARAM_L     2
#define  MAXROWLEN   80   //一行显示的最多字符数

int     g_leave_len = MAXROWLEN;
int     g_maxlen;

//错误处理函数

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
}
void display_single(char*name)
{
  int i,len;
  //如果本行不足以打印一个文件名则换行
  if(g_leave_len < g_maxlen) {
    printf("\n");
    g_leave_len= MAXROWLEN;
  }

  len = strlen(name);
  len  = g_maxlen-len;
  printf("%-s",name);

  for(i=0;i<len;i++){
    printf(" ");
  }
  printf(" ");
  //下面的2指示空格
  g_leave_len-=(g_maxlen+2);
}

//根据命令行参数和完整路径名显示目标文件

void display(int flag,char*pathname)
{
  int     i,j;
  struct stat   buf;
  char    name[NAME_MAX+1];

  //从路径中解析出文件名
  for (i=0,j=0;i<strlen(pathname);i++){
    if(pathname[i]=='/'){
      j=0;
      continue;
    }
    name[j++]=pathname[i];
  }
  name[j]='\0';
  //用lstat而不是stat以方便解析链接文件
  if(lstat(pathname,&buf)==-1){
    my_err("stat",__LINE__);
  }

  switch (flag) {
    case PARAM_NONE:
      if(name[0]!='.'){
        display_single(name);
      }
      break;
    case PARAM_A:
      display_single(name);
      break;
    case PARAM_L :
      if(name[0]!='.'){
        display_attribute(buf,name);
        printf(" %-s\n",name);
      }
        break;
    case PARAM_A+PARAM_L:
      display_attribute(buf,name);
      printf(" %-s\n",name);
      break;

    default:
      break;
    }
}

void display_dir(int flag_param,char *path)
{
  DIR     *dir;
  struct dirent   *ptr;
  int     i,count =0;
  char ** filenames;



  //获取该目录下文件总数和最长的文件名
  dir = opendir(path);
  if(dir == NULL) {
    my_err("opendir",__LINE__);
  }
  while((ptr=readdir(dir))!=NULL){
    if(g_maxlen<strlen(ptr->d_name))
        g_maxlen = strlen(ptr->d_name);
        count++;
  }
  closedir(dir);

    filenames=(char**)malloc(count*sizeof(char));
    for(i=0;i<count;i++)
    filenames[i]=(char*)malloc(g_maxlen*sizeof(char));

    int j,len = strlen(path);
    //获取该文件下所有文件名
    dir = opendir(path);
    for(i = 0;i<count; i++){
      ptr = readdir(dir);
      if(ptr==NULL){
        my_err("readdir",__LINE__);
      }
      strncpy(filenames[i],path,len);
      filenames[i][len] = '\0';
      strcat(filenames[i],ptr->d_name);
      filenames[i][len+strlen(ptr->d_name)]='\0';
  }

//冒泡排序存储



//
for(i=0;i<count;i++)
{
  display(flag_param,filenames[i]);
}

closedir(dir);
//如果命令行中没有-l，打印一个换行符号
  if((flag_param)==0)
  printf("\n");
}


int main(int argc,char**argv)
{
  int     i,j,k,num;
  char    path[1000+1];
  char    param[32];  //保存命令行参数
  char    flag_param=PARAM_NONE;
  struct  stat   buf;

//命令行的解析
  j=0;
  num=0;
  for(i=1;i<argc;i++)
  {
    if(argv[i][0]=='-'){
      for(k=1;k<strlen(argv[i]);k++,j++){
        param[j] = argv[i][k];   //获取-后面的参数不存到数组param中
      }
      num++;  //保存‘-’的个数
    }
  }

//只支持参数a和i，如果含有其他参数选项就报错
for(i=0;i<j;i++){
  if(param[i]=='a'){
    flag_param |= PARAM_A;
    continue;
  }else if(param[i]=='l'){
    flag_param|=PARAM_L;
    continue;
  }else{
    printf("my_ls:invalid opption option -%c\n",param[i]);
    exit(1);
  }
}
param[j]= '\0';

//如果没有输入文件名或者目录名，就显示当前目录
if((num+1)==argc) {
  strcpy(path,"./");
  path[2]='\0';
  display_dir(flag_param,path);
  return 0;
}
i=1;
do{
  if(argv[i][0]=='-'){
    i++;
    continue;
  }else{
    strcpy(path,argv[i]);

    //如果目标文件名或目录不存在，报错并退出程序
    if(stat(path,&buf) == -1)
      my_err("stat",__LINE__);

      if( S_ISDIR(buf.st_mode)){
        //argv[i]是一个目录，如果最后一个字符不是‘/’，加上‘/’
        if(path[strlen(argv[i])-1] !='/'){
          path[strlen(argv[i]) ] = '/';
          path[strlen(argv[i]+1)] ='\0';
        }
        else
          path[strlen(argv[i])] = '\0';

          display_dir(flag_param,path);
          i++;
      }
  }
}
while(i<argc);
return 0;

}
