#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

	int n;
	char buf[1024];

	while(1){
		if(n = read(/*stdin*/0, buf, 1024) > 0){
			buf[n] = '\0';
			printf("ctr receive: %s \n", buf);
			
			if(!strcmp(buf, "e")) exit(0);
			if(!strcmp(buf, "g")) {
				printf("My pid is %d \n", getpid());
				sleep(3);
				exit(0);
			}
		}
	}
	return 0;
}
