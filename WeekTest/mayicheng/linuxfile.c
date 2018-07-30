#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef struct file{
int fid;
char filename[128];
int mode;
int size;
char data[256];
struct file *next;
}myfile;
myfile *head = NULL;
void comeinopen(char *s)
{
	if(chdir(s) == -1)
	{
		//printf("%s\n",strerror(errno));
		return;
	}
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	if((dir = opendir(s)) == NULL)
	{
		//printf("%s\n",strerror(errno));
		return;
	}
	while((ptr = readdir(dir)) != NULL)
	{
		if((strcmp(".","ptr->d_name") == 0) || (strcmp("..",ptr->d_name) == 0))
		continue;
		struct stat buf;
		if(stat(ptr->d_name,&buf) == -1)
		{/*printf("111%s\n",strerror(errno));*/continue;}
		if(S_ISDIR(buf.st_mode) == 1)
		{
			char n[100] = { 0 };
			strcpy(n,s);
			n[strlen(n) - 1] = '/';
			strcat(n,ptr->d_name);
			comeinopen(n);
		}
	//	chdir("..");
		
		myfile *p = head;
		while(p->next)
		{
			p = p->next;
		}
		p->next = (myfile *)calloc(sizeof(myfile),1);
		(p->next)->fid = buf.st_dev;
		strcpy((p->next)->filename,ptr->d_name);
		(p->next)->mode = buf.st_mode;
		(p->next)->size = buf.st_size;
		int fd = open(ptr->d_name,O_RDONLY);
		read(fd,(p->next)->data,256);
		close(fd);
	}
}
void allfiles()
{
	int fd = open("allfiles.dat",O_RDWR | O_CREAT | O_TRUNC,"0666");
	if(fd == -1)
	{printf("%s\n",strerror(errno));return;}
        myfile *p = head->next;
        while(p)
        {
		write(fd,(int *)p->fid,1);
		write(fd,p->filename,strlen(p->filename) + 1);
		write(fd,(int *)p->mode,1);
		write(fd,(int *)p->size,1);
		write(fd,p->data,strlen(p->data) + 1);
                p = p->next;
        }
}
int main()
{
        head = (myfile *)calloc(sizeof(myfile),1);
	char *s = "/lib";
	comeinopen(s);
	myfile *p = head->next;
	        while(p)
        {
		printf("%s\n",p->filename);
                p = p->next;
        }

	allfiles();
	return 0;
}
