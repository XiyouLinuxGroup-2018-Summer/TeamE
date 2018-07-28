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
 void display_single(int flag,char*name)
 {

 }

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
