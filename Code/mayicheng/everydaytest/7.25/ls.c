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

void printsss(int a)
{
	if(a == 0)
	{
		printf("---");
	}
	if(a == 1)
	{
		printf("--x");
	}
	if(a == 2)
	{
		printf("-w-");
	}
	if(a == 3)
	{
		printf("-wx");
	}
	if(a == 4)
	{
		printf("r--");
	}
	if(a == 5)
	{
		printf("r-x");
	}
	if(a == 6)
	{
		printf("rw-");
	}
	if(a == 7)
	{
		printf("rwx");
	}
}
int main(int argc,char **argv)
{
	if(argc == 1)
	{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		DIR *p = NULL;
		struct dirent *prt = NULL;
		p = opendir(c);
		if(p == NULL)
		{
			return 0;
		}
		while((prt = readdir(p)) != NULL)
		{
			if(strcmp(prt->d_name,".") == 0)
			continue;
			if(strcmp(prt->d_name,"..") == 0)
			continue;
			printf("%s  ",prt->d_name);
		}
	}
	if(argc == 2)
	{
		int t = strlen(argv[1]);
		if(t == 3)
		{
		char c[100] = { 0 };
                getcwd(c,sizeof(c));
                DIR *p = NULL;
                struct dirent *prt = NULL;
                p = opendir(c);
                if(p == NULL)
                {
                        return 0;
                }
                while((prt = readdir(p)) != NULL)
                {
                        if(strcmp(prt->d_name,".") == 0)
                        continue;
                        if(strcmp(prt->d_name,"..") == 0)
                        continue;
			struct stat buf;
			int m = stat(prt->d_name,&buf);
			if(m == -1)
			{
				printf("%s\n",strerror(errno));
				return 0;
			}
			//printf("%o",buf.st_mode);
			int t = buf.st_mode % (64 * 8);
			if(S_ISREG(buf.st_mode) == 1)
			{
				printf("-");
			}
			else
			{
				printf("d");
			}
			//printf("t = %d\n",t);
			printsss(t/64);
			printsss(t/8 - t / 64 * 8);
			printsss(t % 8);
			printf(". ");
			printf("%d ",buf.st_nlink);
			struct passwd *psd;
			struct group *grp;
			psd = getpwuid(buf.st_uid);
			grp = getgrgid(buf.st_gid);
			printf("%s ",psd->pw_name);
			printf("%s ",grp->gr_name);
			//printf("%d ",buf.st_uid);
			//printf("%d ",buf.st_gid);
			printf("%6d ",buf.st_size);
			//printf("%s ",ctime(&buf.st_atime));
			char mm[100] = { 0  };
			strcpy(mm,ctime(&buf.st_atime));
			mm[strlen(mm) - 1] = '\0';
			printf("%s111 ",mm);
			printf("%s\n",prt->d_name);
			
			
                }

		}
	}
	if(argc == 3)
	{
		
	}
	return 0;
}
