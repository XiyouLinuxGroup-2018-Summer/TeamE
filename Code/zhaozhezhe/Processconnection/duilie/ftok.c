#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>

int main(void){
	int i;
	//获取键值，准备创建消息队列
	for(i = 1; i <= 5; i++){
		printf("key[%d] = %ul \n", i, ftok(".", i));
	}
	exit(0);
}
