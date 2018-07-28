/*************************************************************************
	> File Name: file.c
	> Author:
	> Mail:
	> Created Time: 2018年07月28日 星期六 17时07分34秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>


typedef struct file{
  int fid;    //文件编号
  char filename[128];
  int mode;
  int size;
  char data[256];
  struct file *next;
}myfile;

myfile head = NULL;

int main()
{
    myfile *p,*q;
    DIR   * dir;
    struct stat buf;
    struct dirent *ptr;
    int num =1;
    int fd;
    char filedata[300];


    chdir("/");
    opendir("/bin");
    if(dir == NULL){
      perror("opendir");
      return -1;
    }

    //创建文件信息链表
    head->next=NULL
    q=head;
    fd =open("./allfiles.dat",O_RDWR);
    while((ptr=readdir(dir))!=NULL)
    {
        lstat(ptr->d_name,&buf);
        if(S_ISDIR(st_mode))
        continue;
        p=(myfile*)malloc(sizeof(myfile));
        p->fid=num;
        write(fd,&num,4);
        num++;
        strcpy(filename,ptr->d_name);
        write(fd,ptr->d_name,256);
        p->mode=buf.st_mode;
        write(fd,&buf.st_mode,12);
        p->size=buf.st_size;
        write
        //fd=open("ptr->d_name")
        //read();
        p->next=NULL;
        q->next = p;
    }





}
