#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <sys/sysmacros.h>

int File_ID = 1;
typedef struct file{
	int fid;
	char filename[128];
	int mode;
	int size;
	char data[256];
	struct file *next;
}myfile;
myfile *Creat_in_node(struct stat _stat, myfile *_myfile, struct dirent *_dirent){
	int mode=0, i = 0, j;
	char rd[256];
	if(_myfile == NULL){
	_myfile = (myfile *)malloc(sizeof(myfile));
	_myfile->fid = File_ID++;
	strncpy(_myfile->filename,_dirent->d_name,127);
	_myfile->filename[127] = 0;
	_myfile->mode = _stat.st_mode;
	_myfile->size = _stat.st_size;
	int fd = open(_dirent->d_name, O_RDONLY);
	int n;
	n = read(fd, rd, 256);
	rd[255] = 0;
	strcpy(_myfile->data,rd);
	}else{
		myfile *my = (myfile *)malloc(sizeof(myfile));
		myfile *file;
		file = _myfile;
		while(file->next != NULL){
			file = file->next;
		}
		file->next = my;
		my->next = NULL;
		_myfile->fid = File_ID++;
		strncpy(_myfile->filename,_dirent->d_name,127);
		_myfile->filename[127] = 0;
		_myfile->mode = _stat.st_mode;
		_myfile->size = _stat.st_size;
		int fd = open(_dirent->d_name, O_RDONLY);
		int n;
		n = read(fd, rd, 256);
		rd[255] = 0;
		strcpy(_myfile->data,rd);
	}

	return _myfile;
}
int main(void){

	DIR *_dir;
	struct dirent *_dirent;
	struct stat _stat;
	myfile *_myfile = NULL;
	_dir = opendir("/lib");
	while((_dirent = readdir(_dir)) != NULL){
		struct stat _stat;
		stat(_dirent->d_name, &_stat);
		if(S_ISDIR(_stat.st_mode)){
			;
		}else{
		//插入链表
		_myfile = Creat_in_node(_stat, _myfile, _dirent);
		int fd;
		fd = ();
		}
	}
	return 0;

}
