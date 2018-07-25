#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//锁
int Set_lock(int fd, struct flock *lock, struct flock *lock_test){
	if(fcntl(fd, F_SETLK, lock) == 0){
		if(lock->l_type == F_RDLCK){
			printf("Set read lock success! PID: %d \n", getpid());
		}else if(lock->l_type == F_WRLCK){
			printf("Set write lock success! PID: %d \n", getpid());
		}else if(lock->l_type == F_UNLCK){
			printf("relese lock! PID : %d \n", getpid());
		}
	}else{
		printf("Set lock failed \n");
		return -1;
	}

	return 0;
}
#if 0
//测试锁
int Test_lock(int fd, struct flock *lock, struct flock *lock_test)
{
	if(fcntl(fd, F_SETLK, lock) == 0){
		printf("lock can be set in fd\n");
		lock->l_type = F_UNLCK;
		fcntl(fd, F_SETLK, lock);
		return 0;	
	}else if(lock->l_type != F_UNLCK){
		if(lock->l_type == F_RDLCK){
			printf("can't set lock, alreadly set read lock PID: %d\n", lock->l_pid);
		}else if(lock->l_type == F_WRLCK){
			printf("can't set lock, alreadly set write lock PID: %d\n", lock->l_pid);	
		}


	}else{
		printf("Test set failed\n");
		return -1;	
	}
}
#endif
int Test_lock(int fd, struct flock *lock, struct flock *lock_test){
	if(fcntl(fd, F_GETLK, lock) == 0){
		if(lock->l_type == F_UNLCK){
			printf("lock can be set in fd\n");
			return 0;
		}else{ 
			if(lock->l_type == F_RDLCK){
				printf("can't set lock, alreadly set read lock PID: %d\n", lock->l_pid);
			}else if(lock->l_type == F_WRLCK){
				printf("can't set lock, alreadly set write lock PID: %d\n", lock->l_pid);	
		}
			return -2;
		}
	}else{
		printf("Can't lock\n");
		return -1;
	}
}
int main(void){

	int fd1;
	int fd2;
	//int fd3;
	//int fd4;
	struct flock lock;
	struct flock lock_test;
	char read_buf[64];

	if((fd1 = open("/home/zhe/桌面/copy1", O_RDONLY)) == -1){
		perror("open file failed");
		exit(0);
	}

	//对结构体置0
	memset(&lock, 0, sizeof(struct flock));
	
	//文件里有东西，不再进行写入操作。
	
	//设置锁的范围，以下是设置整个文件加锁。
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_start = 0;
	
	lock_test.l_whence = SEEK_SET;
	lock_test.l_len = 0;
	lock_test.l_start = 0;

	//设置锁的类型，读锁。
	lock.l_type = F_RDLCK;
	
	if(Test_lock(fd1, &lock, &lock_test) == 0){
		lock.l_type = F_RDLCK;
		Set_lock(fd1, &lock, &lock_test);
	}
	memset(read_buf, 0, sizeof(read_buf));

	lseek(fd1, 0, SEEK_SET);
	fd2 = read(fd1, read_buf, sizeof(read_buf));
	read_buf[fd2] = '\0';
	printf("Read_buf is: %s.\n", read_buf);
	
	memset(read_buf, 0, sizeof(read_buf));
	getchar();

	lock.l_type = F_WRLCK;
	if(Test_lock(fd1, &lock, &lock_test) == 0){
		lock.l_type = F_WRLCK;
		Set_lock(fd1, &lock, &lock_test);
	}


	lock.l_type = F_UNLCK;
	Set_lock(fd1, &lock, &lock_test);
	close(fd1);

	return 0;
}
