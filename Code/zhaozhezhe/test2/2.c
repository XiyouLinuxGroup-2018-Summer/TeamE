#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>



int main(int argc, char *argv[]){
    for (int i = 0; i < 2; i++){
        pid_t pid = fork();
        printf("hello, world?\n");
    }
    return 0;
}
