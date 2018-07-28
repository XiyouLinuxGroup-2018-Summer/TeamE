/*************************************************************************
	> File Name: show_files.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月25日 星期三 17时37分13秒
 ************************************************************************/

#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int my_readir(const char * path)
{
    DIR         *dir;
    struct dirent       *ptr;

    dir=opendir(path);

    while((ptr=readdir(dir))!=NULL){
        printf("file name:%s\n",ptr->d_name);
    }
    closedir(dir);

    return 0;
}

int main(int argc,char** argv)
{
    if(argc<2){
        printf("listfile <target path>\n");
        exit(1);
    }

    if(my_readir(argv[1])<0){
        exit(1);
    }
    return 0;
}
