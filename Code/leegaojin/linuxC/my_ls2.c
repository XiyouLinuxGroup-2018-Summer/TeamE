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


#define  MAXROWLEN   80   //一行显示的最多字符数




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

  printf("%-s\n",name);


}
int main(int argc,char**argv)
{
    struct stat buf;
    DIR     *dir;
    struct dirent     *ptr;
    int len,i,param=0;
    if(argv[1][0]=='-')
    {
      len= strlen(argv[1]);
      for(i=0;i<len;i++){
        if(argv[1][i]=='a'){
          param+=1;
        }else if(argv[1][i]=='l'){
          param+=2;
        }
      }
    }

    if(chdir(argv[2])<0){
      my_err("chdir",__LINE__);

    }
    //getcwd(buf,512);
    //printf("%s\n",buf);
    switch (param) {
      case 1:if((dir = opendir(argv[2]))==NULL)  {
        my_err("opendir",__LINE__);
        exit(1);
      }
      while((ptr = readdir(dir))!=NULL){
              if(lstat(ptr->d_name,&buf)==-1){
            my_err("stat",__LINE__);
          }
          display_single(ptr->d_name);
          //display_attribute(buf,ptr->d_name);
      };break;
      case 2:if((dir = opendir(argv[2]))==NULL)  {
        my_err("opendir",__LINE__);
        exit(1);
      }
      while((ptr = readdir(dir))!=NULL){
              if(lstat(ptr->d_name,&buf)==-1){
            my_err("stat",__LINE__);
          }
          //display_single(ptr->d_name);
          display_attribute(buf,ptr->d_name);
      }break;
      default:break;
    }







    return 0;



}
