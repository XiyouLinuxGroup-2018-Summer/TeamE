/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 18时51分52秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <linux/sem.h>
#include <unistd.h>

#define max_resource 5

int main(void){

    key_t key;
    int semid;
    struct sembuf sbuf = {0, -1, IPC_NOWAIT};
    union semun semopts;

    if((key = ftok(".", 's')) == -1){
        printf("ftok error.\n");
    }

    if((semid = semget(key, 1, IPC_CREAT | 0666)) == -1){
        printf("semget error.\n");
        exit(1);
    }

    semopts.val = max_resource;

    if(semctl(semid, 0, SETVAL, semopts) == -1){
        printf("semctl error.\n");
        exit(1);
    }

    while (1){
        if(semop(semid, &sbuf, 1) == -1){
            printf("semop error.\n");
            exit(1);
        }

        sleep(3);
    }
}
