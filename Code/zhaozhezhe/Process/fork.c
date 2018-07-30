#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	
	//创建子进程。
	int Child = fork();
	if(Child < 0){
		printf("error %d\n", __LINE__);
		exit(0);
	}else if(Child == 0){
		printf("I am the Children.\n");
	}else{
		printf("I am thd Father.\n");
	}
	return 0;
}
