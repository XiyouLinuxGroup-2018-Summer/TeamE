# 进程之间的通信

	进程之间的通信包括`管道`、`命名管道`、`消息队列`、`信号量`、`共享内存`。

	管道的工作方式为半双工（即管道工作时，只有一个端可以进行工作、只能读或者写，不能同时写和读）。

![img](http://images2015.cnblogs.com/blog/323808/201603/323808-20160311093936866-901519688.png)

	两个管道的组合可以实现一个全双工的一个工作方式（可以进行进程之间的通信）。

![img](http://images2015.cnblogs.com/blog/323808/201603/323808-20160311094030069-935122142.png)

```C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

/*写操作*/
void write_to_pipe(int fd){
	char message[100];
	read(fd, message, 100);
	printf("read from pipe :%s\n", message);
}
/*读操作*/
void read_to_pipe(int fd){
	char message[1000];
	peintf("Input your message!\n");
	scanf("%s", message);
	write(fd, message, strlen(message)+1);
}

int main(void){
	int fd[2];
	pid_t pid;
	int stat_val;

	/*创建一个管道*/
	if(pipe (fd)){
		printf("Create process failed!\n");
		exit(1);
	}

    /*fork在创建管道之后，因为在fork在前父子进程继承的不一样*/
	pid = fork();

	if(pid < 0){
		printf("Pid is error.\n");
		exit(1);
	}else if(pid == 0){
		close(fd[1]);
		write_to_pipe(fd[0]);
	}else if(pid > 0){
		close(fd[0]);
		read_to_pipe(fd[1]);
		wait(&stat_val);
		exit(0);
	}

	return 0;
}
```



	两个管道实现的全双工对于我们的一个进程之间的通信很重要。

```	C
/*两个进程使用两个管道最终形成了进程之间的通信*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

void child_rw_pipe(int readfd, int writefd){
	char *message = "This is the message1, from child";
	write(writefd, message, strlen(message)+1);
	
	char message2[100];
	read(readfd, message2, 100);
	printf("Child read the message :%s \n", message2);
}
void parent_rw_pipe(int readfd, int writefd){
	char *message = "This is the message2, from parent";
	write(writefd, message, strlen(message)+1);
	
	char message2[100];
	read(readfd, message2, 100);
	printf("parent read the message :%s \n", message2);
}
int main(void){
	int pipe1[2], pipe2[2];
	pid_t pid;
	int stat_val;

	printf("realize full-duplex communication.\n");
	/*创建一个管道*/
	if(pipe(pipe1)){
		printf("Create pipe1 failed!\n");
		exit(1);
	}
	if(pipe(pipe2)){
		printf("Create pipe2 failed!\n");
		exit(1);
	}

	pid = fork();

	if(pid < 0){
		printf("Pid is error.\n");
		exit(1);
	}else if(pid == 0){
		close(pipe1[1]);
		close(pipe2[0]);
		child_rw_pipe(pipe1[0], pipe2[1]);
	}else if(pid > 0){
		close(pipe1[0]);
		close(pipe2[1]);
		parent_rw_pipe(pipe2[0], pipe1[1]);
		wait(&stat_val);
		exit(0);
	}

	return 0;
}
```

