/*************************************************************************
	> File Name: read.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 20时14分13秒
 ************************************************************************/

#include "sharemem.h"

int main(void){
    int semid, shmid;
    char *shmaddr;

    if((shmid = createshm(".", 'm', shm_size)) == -1){
        exit(1);
    }

    if((shmaddr = shmat(shmid, (char *)0, 0)) == (char *)-1){
        exit(1);
    }

    if((semid = opensem(".", 's')) == -1){
        exit(1);
    }

    while (1){
        printf("readder: ");
        wait_sem(semid, 0);
        sem_p(semid, 0);

        printf("%s\n", shmaddr);
        sleep(10);

        sem_v(semid, 0);
        sleep(10);
    }
}

