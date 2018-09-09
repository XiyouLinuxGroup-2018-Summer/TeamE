/*************************************************************************
	> File Name: write.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 20时06分51秒
 ************************************************************************/

#include "sharemem.h"

int main(void){
    int semid,shmid;
    char *shmaddr;
    char write_str[shm_size];

    if((shmid = createshm(".", 'm', shm_size)) == -1){
        exit(1);
    }

    if((shmaddr = shmat(shmid, (char *)0, 0)) == (char *)-1){
        printf("shmat error.\n");
        exit(1);
    }
    
    if((semid = createsem(".", 's', 1, 1)) == -1){
        exit(1);
    }

    while(1){
        wait_sem(semid, 0);
        sem_p(semid, 0);
        printf("writer:");
        fgets(write_str,1024, stdin );
        int len = strlen(write_str)-1;

        strcpy(shmaddr, write_str);
        sleep(10);
        
        sem_v(semid, 0);
        sleep(10);
    }
}

