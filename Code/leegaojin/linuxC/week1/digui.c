/*************************************************************************
	> File Name: digui.c
	> Author:
	> Mail:
	> Created Time: 2018年07月26日 星期四 17时29分17秒
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

 #define flag_N 0
 #define flag_A 1
 #define flag_L 2
 #define flag_R 4

 void my_err(const char *err_string,int line)
 {
   fprintf(stderr, "line:%d ",line);
   perror(err_string);
   //exit(1);
   return;
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
 //void display_single(int flag,char*name)
 //{

 //}

 void my_digui(char *dir_name)
 {
   struct stat buf;
   char buf1[100];
   DIR     *dir;
   struct dirent     *ptr;
   int len,i,param=0;

   if(chdir(dir_name)<0)
   {
     my_err("chdir",__LINE__);
   }
    if(getcwd(buf1,512)<0){
       my_err("getcwd",__LINE__);
     }
    printf("%s:\n",buf1);


    dir = opendir(buf1);
   if(dir==NULL)
   {
     my_err("opendir",__LINE__);
     exit(1);
   }

   while((ptr = readdir(dir))!=NULL)
   {
           if(lstat(ptr->d_name,&buf)==-1)
           {
              my_err("stat",__LINE__);
           }
           if(strcmp(ptr->d_name,"..")==0)
           continue;
           if(strcmp(ptr->d_name,".")==0)
           continue;
           if(S_ISDIR(buf.st_mode)){

                my_digui(ptr->d_name);
           }
           else if(!S_ISDIR(buf.st_mode))
            {
              display_attribute(buf,ptr->d_name);
                printf("%s\n",ptr->d_name);
            }
    }
    if(chdir("..")<0)
    {
      my_err("chdir",__LINE__);
    }
    closedir(dir);
    return;
 }

 int main(int argc,char**argv)
 {
       my_digui(argv[1]);
       return 0;
  }
