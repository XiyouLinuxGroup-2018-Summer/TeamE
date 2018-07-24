# LINUX下文件的操作

以下均已ubuntu 18.04 LTS作为环境。

1. 文件的打开（open）

   - 打开文件的函数原型：

      **open(const char *pathname, int flags);**

      **open(const char *pathname, int flags, mode_t mode);**

   - 打开文件函数需要的头文件：

      **#include  <sys/types.h>**

      **#include  <sys/stat.h>**
      **#include  <fcntl.h>**

   文件的打开需要**文件的绝对或相对路径字符串**，还有文件打开的方式。

   一般常用有以下三种。

   | 对应的标志 | 含义                                                         |
   | ---------- | ------------------------------------------------------------ |
   | O_RDONLHY  | 只读                                                         |
   | O_WRONLY   | 只写                                                         |
   | O_RDWR     | 读写                                                         |
   | O_CREAT    | 文件不存在就创建这个文件（需要使用mode_t mode 参数进行文件权限的设置） |
   | O_TRUNC    | 文件存在并且以可写的形式打开时，此标志将会把文件长度清零，即原文件格式化 |
   | O_APPEND   | 写入的数据添加到文件的尾部                                   |

   并且前三种的标志互相冲突，不可以同时进行**只读、只写、读写操作**

2. 文件的读取（read）

   - 文件读取的函数原型：

     **read(int fd, void *buf, size_t count);**

   - 需要用到的头文件：

     **#include <unistd.h>**

   文件的读取需要注意的是每个参数的意义：

   ​	fd 对应的是 打开文件时的文件描述符，文件描述符对应的是一个int值。当打开文件失败时，open函数对应返回 -1 作为读取失败的标志。

   ​	buf 是文件读取写入的字符串。

   ​	count 为一次读取的字节个数。

3. 文件的写入（write）

   - 文件写入的函数原型：

     **write(int fd, const void *buf, size_t count);**

   - 需要用到的头文件：

     **#include <unistd.h>**

     fd 对应的是 打开文件时的文件描述符，文件描述符对应的是一个int值。当打开文件失败时open函数对应返回 -1 作为读取失败的标志。

     

   		buf 是写入文件的字符串。
   		
   		count 为一次写入的字节个数。

4. 文件的关闭（close）

   - 关闭文件的原型：

     **close(int fd);**

   - 需要的头文件：

     **#include <unistd.h>**

     关闭文件的读取，只需要文件的描述符即可。关闭文件应在所有文件操作之后。
     
```C
//利用以上文件的操作，可以完成对一个的复制功能。
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
	if((fd2 = open(argv[2], O_WRONLY)) == -1){//在这个判断条件里文件描述符不能用括号括！！！！！
		
		perror("open file2 failed\n");
	}

//	fd2 = open(argv[2], O_WRONLY);

	while(success){
		//成功读取的字符个数
		//int sussess;
		success = read(fd1, copy, 100);
		write(fd2, copy, success);	
	}
	//关闭两个文件
	close(fd1);
	close(fd2);
	return 0;
}
```


5. 文件指针的移动（lseek）

   - 指针移动函数的原型：

     **lseek(int fd, off_t offset, int whence);**

   - 需要的头文件

     **#include <sys/types.h>**

     **#include <unistd.h>**

     | 移动的三种状态 | 操作及意义                                                   |
     | -------------- | ------------------------------------------------------------ |
     | SEEK_SET       | 将文件指针移动到文件的开始处，offset是指针相对SEEK_SET的偏移位置，所以此时offset只有正值，即只能向文件的末尾偏移。 |
     | SEEK_CUR       | 对于当前文件指针指向进行偏移，可正可负                       |
     | SEEK_END       | 文件指针指向文件的尾部，只能读取负值，即向文件的开头偏移。   |

     

