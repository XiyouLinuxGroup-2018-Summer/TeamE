/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 18时59分59秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/sem.h>

int main(void){
    key_t key;
    int semid, semval;
    union semun semopts;

    if((key = ftok(".", 's')) == -1){
        printf("ftok error.\n");
        exit(1);
    }

    if((semid = semget(key, 1, IPC_CREAT|0666)) == -1){
        printf("semget error.\n");
        exit(1);
    }

    while(1){
        
        if((semval = semctl(semid, 0, GETVAL, 0)) == -1){
            printf("semctl error.\n");
            exit(1);
        }

        if(semval > 0){
            printf("Still %d resources can be used\n", semval);
        }else{
            printf("No more resources can be used\n");
            break;
        }
        sleep(3);
    }

    return 0;
}
