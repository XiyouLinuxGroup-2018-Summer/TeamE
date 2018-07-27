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
void a()
{
	printf("a = %s\n",strerror(errno));
}
int pqq = 0;
long long ssum = 0;

char cnum[3] = { 0 };	//0中保存a，1中保存l，2中保持R 

char *thisisthe = NULL;
typedef struct dongtai
{
	char c[50];
}aa;
struct dongtai *dt;
void panduan(char *c)
{
	int i = 1;
	if(c[0] == '/')
	{
	  thisisthe = (char *)calloc(sizeof(100),1);
	  strcpy(thisisthe,c);
	  return;
	}
		if(c[0] != '-' )
		{
			printf("ls: 无法访问%s： 没有那个文件或目录\n",c);
			exit(0);
		}
	for(i = 1;c[i] != '\0';i++)
	{
		if(c[i] != 'R' && c[i] != 'l' && c[i] != 'a')
		{ 
			printf("ls:无效选项 -- c[%c]\n",c[i]);
			printf("Try 'ls --help'for more information\n");
			exit(0); 
		}
		if(c[i] == 'R')
		cnum[2] = 1;
		if(c[i] == 'a')
		cnum[0] = 1;
		if(c[i] == 'l')
		cnum[1] = 1;
	}
}
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
void argc111()	//一个参数时，这样 
{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		DIR *p = NULL;
		struct dirent *prt = NULL;
		p = opendir(c);
		if(p == NULL)
		{
			return;
		}
		while((prt = readdir(p)) != NULL)
		{
			if(strcmp(prt->d_name,".") == 0)
			continue;
			if(strcmp(prt->d_name,"..") == 0)
			continue;
			if(prt->d_name[0] == '.')
			continue;
			printf("%s  ",prt->d_name);
		}
		printf("\n");	
}
void printff00(char *c)
{
                 if(chdir(c) == -1)
                 {/*a();printf("c = %s\n",c);*/return;}
                DIR *p = NULL;
                struct dirent *prt = NULL;
                p = opendir(c);
                if(p == NULL)
                {
                        return;
                }
                while((prt = readdir(p)) != NULL)
                {
                      if(strcmp(prt->d_name,".") == 0)
                      continue;
                      if(strcmp(prt->d_name,"..") == 0)
                      continue;
                      if(prt->d_name[0] == '.')
                      continue;
                     printf("%s  ",prt->d_name);
                }
                printf("\n");
}
void printff1(char *c)
{
   		 if(chdir(c) == -1)
   		 {/*a();printf("c = %s\n",c);*/return;}
		DIR *p = NULL;
		struct dirent *prt = NULL;
		p = opendir(c);
		if(p == NULL)
		{
			return;
		}
		while((prt = readdir(p)) != NULL)
		{
//			if(strcmp(prt->d_name,".") == 0)
//			continue;
//			if(strcmp(prt->d_name,"..") == 0)
//			continue;
			printf("%s  ",prt->d_name);
		}
		printf("\n");		
}
void printff2(char *c)
{
    if(chdir(c) == -1)
    {/*a();printf("c = %s\n",c);*/return;}
    DIR *p = NULL;
    struct dirent *prt = NULL;
    p = opendir(c);
    if(p == NULL)
    {
    	return;
    }
    while((prt = readdir(p)) != NULL)
    {
    if(strcmp(prt->d_name,".") == 0)
    continue;
    if(strcmp(prt->d_name,"..") == 0)
    continue;
	if(prt->d_name[0] == '.')
	continue;
	struct stat buf;
	int m = stat(prt->d_name,&buf);
	if(m == -1)
	{
		printf("%s\n",strerror(errno));
		return;
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
	printf("%s ",mm);
	printf("%s\n",prt->d_name);
    }
	
}
void printff3(char *c)
{
    if(chdir(c) == -1)
    {/*a();printf("c = %s\n",c);*/return;}
    DIR *p = NULL;
    struct dirent *prt = NULL;
    p = opendir(c);
    if(p == NULL)
    {
    	return;
    }
    while((prt = readdir(p)) != NULL)
    {
//    if(strcmp(prt->d_name,".") == 0)
//    continue;
//    if(strcmp(prt->d_name,"..") == 0)
//    continue;
	struct stat buf;
	int m = stat(prt->d_name,&buf);
	if(m == -1)
	{
		printf("%s\n",strerror(errno));
		return;
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
	printf("%s ",mm);
	printf("%s\n",prt->d_name);
    }
		
}
void r(const char *c)
{
	DIR *dir;
	struct dirent *ptr;
	if(chdir(c) == -1)
	{/*a();printf("c = %s\n",c);*/return;}
	if((dir = opendir(c)) == NULL)
	{a();return;}
	while((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(".",ptr->d_name)==0||strcmp("..",ptr->d_name)==0)
		continue;
		//if(ptr->d_name[0] == '.')
		//continue;
		printf("%s   ",ptr->d_name);
		struct stat buf;
		if(lstat(c,&buf) == -1)
		{a();return;}
		//if(S_ISDIR(buf.st_mode) == 1 && S_ISREG(buf.st_mode) != 1)
		//{
		if(S_ISDIR(buf.st_mode))
		{
			char n[100] = { 0 };
			strcpy(n,c);
			if(n[strlen(n)] != '/')
			{
				n[strlen(n)] = '/';
				strcat(n,ptr->d_name);
			}
			printf("\n");
			r(n);
		}
	}
	chdir("..");		//this
	closedir(dir);
	return;
}
void printff5(const char *c)
{
	DIR *dir;
	struct dirent *ptr;
	if(chdir(c) == -1)
	{/*a();printf("c = %s\n",c);*/return;}
	if((dir = opendir(c)) == NULL)
	{a();return;}
	while((ptr = readdir(dir)) != NULL)
	{
		printf("%s   ",ptr->d_name);
		if(strcmp(".",ptr->d_name)==0||strcmp("..",ptr->d_name)==0)
		continue;
		//if(ptr->d_name[0] == '.')
		//continue;
		struct stat buf;
		if(lstat(c,&buf) == -1)
		{a();return;}
		//if(S_ISDIR(buf.st_mode) == 1 && S_ISREG(buf.st_mode) != 1)
		//{
		if(S_ISDIR(buf.st_mode))
		{
			char n[100] = { 0 };
			strcpy(n,c);
			if(n[strlen(n)] != '/')
			{
				n[strlen(n)] = '/';
				strcat(n,ptr->d_name);
			}
			printf("\n");
			printff5(n);
		}
	}
	chdir("..");		//this
	closedir(dir);
	return;
}
void printff6(const char *c)
{
	DIR *dir;
	struct dirent *ptr;
	if(chdir(c) == -1)
	{/*a();printf("c = %s\n",c);*/return;}
	if((dir = opendir(c)) == NULL)
	{a();return;}
	while((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(".",ptr->d_name)==0||strcmp("..",ptr->d_name)==0)
		continue;
		//if(ptr->d_name[0] == '.')
		//continue;
		struct stat buf;
		if(lstat(c,&buf) == -1)
		{a();return;}
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
		printf("%s ",mm);
		printf("%s\n",ptr->d_name);
		//if(S_ISDIR(buf.st_mode) == 1 && S_ISREG(buf.st_mode) != 1)
		//{
		if(S_ISDIR(buf.st_mode))
		{
			char n[100] = { 0 };
			strcpy(n,c);
			if(n[strlen(n)] != '/')
			{
				n[strlen(n)] = '/';
				strcat(n,ptr->d_name);
			}
			printf("\n");
			printff6(n);
		}
	}
	chdir("..");		//this
	closedir(dir);
	return;
}
void printff7(const char *c)
{
	DIR *dir;
	struct dirent *ptr;
	if(chdir(c) == -1)
	{/*a();printf("c = %s\n",c);*/return;}
	if((dir = opendir(c)) == NULL)
	{a();return;}
	while((ptr = readdir(dir)) != NULL)
	{
		//if(ptr->d_name[0] == '.')
		//continue;
	  	struct stat buf;
		if(lstat(c,&buf) == -1)
		{a();return;}
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
		printf("%s ",mm);
		printf("%s\n",ptr->d_name);
		if(strcmp(".",ptr->d_name)==0||strcmp("..",ptr->d_name)==0)
		continue;
		//if(S_ISDIR(buf.st_mode) == 1 && S_ISREG(buf.st_mode) != 1)
		//{
		if(S_ISDIR(buf.st_mode))
		{
			char n[100] = { 0 };
			strcpy(n,c);
			if(n[strlen(n)] != '/')
			{
				n[strlen(n)] = '/';
				strcat(n,ptr->d_name);
			}
			printf("\n");
			printff7(n);
		}
	}
	chdir("..");		//this
	closedir(dir);
	return;
}
/*
void printff4(char *c)
{
		DIR *p = NULL;
		struct dirent *prt = NULL;
		p = opendir(c);
		if(p == NULL)
		{
			printf("1-%s\n",strerror(errno));
			return;
		}
		while((prt = readdir(p)) != NULL)
		{
		char *nn = calloc(100,1);
		strcpy(nn,c);
		int t = strlen(nn);
		nn[t] = '/';
		nn[t + 1] = '\0';
		strcat(nn,prt->d_name);			
			struct stat buf;
			//printf("%s\n",nn);
			int m = stat(nn,&buf);
			//int m = stat(prt->d_name,&buf);
			if(m == -1)
			{
				printf("nn = %s\n",nn);
				printf("2-%s\n",strerror(errno));
				continue;
			}
	//printf("%o",buf.st_mode);
			if(strcmp(prt->d_name,".") == 0)
	   		continue;
    		if(strcmp(prt->d_name,"..") == 0)
   			continue;
   			if(prt->d_name[0] == '.')
			continue;
	if(S_ISDIR(buf.st_mode) == 1)
	{
				//printff4(nn);
//				if(dt[ssum].c != NULL)
//				{
//					free(dt[ssum].c);
//					dt[ssum].c = 0;
//				}
//				dt[ssum].c = (char *)calloc(strlen(nn) + 1,1);
				strcpy(dt[ssum++].c,nn);
		//printff4(strcat(m,prt->d_name));
//		free(m);
	}
//			if(strcmp(prt->d_name,".") == 0)
//			continue;
//			if(strcmp(prt->d_name,"..") == 0)
//			continue;
			printf("%s  ",prt->d_name);
			free(nn);
		}	
}*/
int main(int argc,char **argv)
{
	dt = calloc(sizeof(aa),8 * 1024 * 1024);
	if(argc == 1)
	{
		argc111();
		return 0;
	}
	int aaa = 1;
	for(aaa = 1;argv[aaa];aaa++)
	{
		panduan(argv[aaa]);
	}
//	int mac = 0;
//	for(;mac < 3;mac++)		//看看到底是什么字符 
//	{
//		printf("cnum = %d",cnum[mac]);
//	}
//
        if(cnum[0] != 1 && cnum[1] != 1 && cnum[2] != 1)
        {
                if(thisisthe)
                {
                	char c[100] = { 0 };
       		        strcpy(c,thisisthe);
                	printff00(c);
		}
                return 0;
        }
	if(cnum[0] == 1 && cnum[1] != 1 && cnum[2] != 1)
	{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
		printff1(c);
		return 0;
	}
	if(cnum[0] != 1 && cnum[1] == 1 && cnum[2] != 1)
	{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
		printff2(c);
		return 0;
	}
	if(cnum[0] == 1 && cnum[1] == 1 && cnum[2] != 1)
	{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
		printff3(c);
		return 0;
	}
	if(cnum[0] != 1 && cnum[1] != 1 && cnum[2] == 1)
	{
		char c[100] = { 0 };
		getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
		r(c);
		/*
		for(;ssum > 0;)
		{//this
			printff4(dt[--ssum].c);
		}
		*/
		printf("\n");
		return 0;
	}
        if(cnum[0] == 1 && cnum[1] != 1 && cnum[2] == 1)
        {
                char c[100] = { 0 };
                getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
                printff5(c);
		printf("\n");
                return 0;
        }
        if(cnum[0] != 1 && cnum[1] == 1 && cnum[2] == 1)
        {
                char c[100] = { 0 };
                getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
                printff6(c);
                return 0;
        }
        if(cnum[0] == 1 && cnum[1] == 1 && cnum[2] == 1)
        {
                char c[100] = { 0 };
                getcwd(c,sizeof(c));
		if(thisisthe)
		{
		  strcpy(c,thisisthe);
		}
                printff7(c);
                return 0;
        }

/*	
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
*/
	return 0;
}
