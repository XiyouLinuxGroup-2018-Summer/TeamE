#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
	

	//成功读取的数据个数，设置为1 ,确保进入循环
	int success = 1;
	//文件描述符定义
	int fd1;
	int fd2;

	//复制中转字符数组
	char copy[100];

	//argv[1]为要复制的文件名，argv[2]为复制的副本文件名
	//先判断参数是否够
	
	if(argc < 3){
		printf("参数不够！\n");
	}
	//再打开文件
	//打开文件失败
	if((fd1 = open(argv[1], O_RDONLY)) == -1){
	
		perror("open file1 failed\n");
		exit(0);
	}
	if((fd2 = open(argv[2], O_WRONLY)) == -1){
		
		perror("open file2 failed\n");
	}

//	fd2 = open(argv[2], O_WRONLY);

	while(success){
		//成功读取的字符个数
		//int sussess;
		success = read(fd1, copy, 100);
		write(fd2, copy, success);	
	}
	
	close(fd1);
	close(fd2);
	return 0;
}
