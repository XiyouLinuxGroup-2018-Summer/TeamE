/*************************************************************************
	> File Name: sharemem.h
	> Author: 
	> Mail: 
	> Created Time: 2018年09月09日 星期日 19时32分24秒
 ************************************************************************/

#ifndef _SHAREMEM_H
#define _SHAREMEM_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#define shm_size 1024

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

int createsem(const char *pathname, int pid, int menbers, int init_val){
    key_t msgkey;
    int index,sid;
    union semun semopts;

    if((msgkey = ftok(pathname, pid)) == -1){
        printf("ftok error.\n");
        return -1;
    }
    if((sid = semget(msgkey, menbers, IPC_CREAT|0666)) == -1){
        printf("semget error.\n");
        return -1;
    }
    
    semopts.val = init_val;

    for(index = 0; index < menbers; index++)
    {
        semctl(sid, index, SETVAL, semopts);
    }

    return sid;
}

int opensem(const char *pathname, int pid){
    key_t msgkey;
    int sid;

    if((msgkey = ftok(pathname, pid)) == -1){
        printf("ftok error.\n");
        return -1;
    }

    if((sid = semget(msgkey, 0, IPC_CREAT|0666)) == -1){
        printf("semget error.\n");
        return -1;
    }
    return sid;
}
int sem_p(int semid, int index){
    struct sembuf buf = {0, -1, IPC_NOWAIT};
    if(index < 0){
        printf("index of array cannot equals a minus value.\n");
        return -1;
    }

    buf.sem_num = index;

    if(semop(semid, &buf, 1) == -1){
        printf("a wrong operation to semaphore occurred.\n");
        return -1;
    }

    return 0;
}
int sem_v(int semid, int index){
    struct sembuf buf = {0, +1, IPC_NOWAIT};
    if(index < 0){
        printf("index of array cannot equals a minus value.\n");
        return -1;
    }

    buf.sem_num = index;

    if(semop(semid, &buf, 1) == -1){
        printf("a wrong operation to semaphore occurred.\n");
        return -1;
    }

    return 0;
}
int sem_delete(int semid){
    return (semctl(semid, 0, IPC_RMID));
}

int wait_sem(int semid, int index){
    while (semctl(semid, index, GETVAL, 0) == 0){
        sleep(1);
    }
    return 1;
}

int createshm(char *pathname, int pid, size_t size){
    key_t shmkey;
    int sid;

    if((shmkey = ftok(pathname, pid)) == -1){
        printf("ftok error.\n");
        return -1;
    }
    if((sid = shmget(shmkey, size, IPC_CREAT | 0666)) == -1){
        printf("shmget call failed.\n");
        return -1;
    }

    return sid;
}
#endif
