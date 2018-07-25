#include <stdio.h>
#include <stdlib.h>
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
#define MAX 1000
void Print(char *route, char *value);
void Print_Jur(char *File_name);
void Print_UG(char *File_name);
void Print_File_Size(char *File_name);
void Print_Time(char *File_name);
void Print_Link(char *File_name);
void Print_No_Paramter(char *route);


//打印链接数
void Print_Link(char *File_name){
	
	struct stat My_Stat;
	stat(File_name, &My_Stat);

	printf(" %lu",My_Stat.st_nlink);
}
//打印最近修改时间
void Print_Time(char *File_name){
	
	struct stat My_Stat;

	char Time_buf[32];

	stat(File_name, &My_Stat);

	strcpy(Time_buf, ctime(&My_Stat.st_mtime));
	Time_buf[strlen(Time_buf)-1] = '\0';
	//strftime(Time_buf, 32, "%b %d  %R", ctime(&My_Stat.st_mtime));
	printf(" %s", Time_buf);
	
}
//打印文件大小
void Print_File_Size(char *File_name){
	
	struct stat My_Stat;

	stat(File_name, &My_Stat);

	printf("%6ld", My_Stat.st_size);
}
//打印文件所属用户和用户组
void Print_UG(char *File_name){

	struct stat My_Stat;
	struct passwd *usr;
	struct group *grp;

	stat(File_name, &My_Stat);
	
	usr = getpwuid(My_Stat.st_uid);
	grp = getgrgid(My_Stat.st_gid);

	printf(" %-4s", usr->pw_name);
	printf("%-4s", grp->gr_name);

}
	

//权限：Jurisdiction
//该函数打印文件的类型和权限。
void Print_Jur(char *File_name){
	struct stat My_Stat;

	stat(File_name, &My_Stat);
	//直接打印权限信息。
	//打印文件类型
	if (S_ISLNK(My_Stat.st_mode)){
		printf("l");
	}else if (S_ISREG(My_Stat.st_mode)){
		printf("-");
	}else if (S_ISDIR(My_Stat.st_mode)){
		printf("d");
	}else if (S_ISCHR(My_Stat.st_mode)){
		printf("c");
	}else if (S_ISBLK(My_Stat.st_mode)){
		printf("b");
	}else if (S_ISFIFO(My_Stat.st_mode)){
		printf("f");
	}else if (S_ISSOCK(My_Stat.st_mode)){
		printf("s");
	}

	//打印权限类型
	//文件所有者读、写、执行权限
	if(My_Stat.st_mode & S_IRUSR){
		printf("r");
	}else{
		printf("-");
	}

	if(My_Stat.st_mode & S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}
	if(My_Stat.st_mode & S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}
	//文件拥有者所在组内读，写，执行权限
	if(My_Stat.st_mode & S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}
	if(My_Stat.st_mode & S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}
	if(My_Stat.st_mode & S_IXGRP){
		printf("r");
	}else{
		printf("-");
	}
	//其他人读、写、执行权限
	if(My_Stat.st_mode & S_IROTH){
		printf("r");
	}else{
		printf("-");
	}
	if(My_Stat.st_mode & S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}
	if(My_Stat.st_mode & S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}

	//文件类型和权限，打印完毕。

}
//打印没有参数的情况，列出当前文件夹下的文件名
void Print_No_Paramter(char *route){

	DIR *My_File_DIR;
	struct dirent *My_Dirent;
	My_File_DIR = opendir(route);
	int n = 0;
	while((My_Dirent = readdir(My_File_DIR))!= NULL){
		//碰到隐藏文件不操作。
		if(My_Dirent->d_name[0] =='.'){
				;
		}else{
			printf(" %-15s", My_Dirent->d_name);
			n++;
			if(n%5==0){
				printf("\n");
			}
		}
	}
	closedir(My_File_DIR);
	
}
void Print(char *route, char *value){

	DIR *My_File_DIR;
	struct dirent *My_Dirent;

	//struct stat My_Stat;
	//参数为空，打印程序所在目录下非隐藏文件。
	
	//打开文件
	My_File_DIR = opendir(route);
	//这里调用会缺失一个文件
	//My_Dirent = readdir(My_File_DIR)
	
	//value[1] 设置为代表 ls -l;
	
	if(value[1] == '\0' && value[0] == '\0'){
		int n = 0;
		while((My_Dirent = readdir(My_File_DIR))!= NULL){
			//碰到隐藏文件不操作。
			if(My_Dirent->d_name[0] =='.'){
				;
			}else{
				printf(" %-15s", My_Dirent->d_name);
				n++;
				if(n%5==0){
					printf("\n");
				}
			}
		}
		
		printf("\n");	
	}else{
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....

		if(value[0] == 'a' && value[1] == '\0'){
			int n = 0;
			while((My_Dirent = readdir(My_File_DIR))!= NULL){
				printf(" %-15s", My_Dirent->d_name);
				n++;
				if(n%7==0){
					printf("\n");
				}

			}
		}

		if(value[0] == 'a' && value[1] == 'l'){

			while(NULL != (My_Dirent = readdir(My_File_DIR))){

				Print_Jur(My_Dirent->d_name);
				Print_Link(My_Dirent->d_name);
				Print_UG(My_Dirent->d_name);
				Print_File_Size(My_Dirent->d_name);
				Print_Time(My_Dirent->d_name);
				printf(" %-15s", My_Dirent->d_name);
				printf("\n");

			}

		}

		if(value[0] == '\0' && value[1] =='l'){
			while((My_Dirent = readdir(My_File_DIR))!= NULL){
				//碰到隐藏文件不操作。
				if(My_Dirent->d_name[0] =='.'){
					;
				}else{

					Print_Jur(My_Dirent->d_name);
					Print_Link(My_Dirent->d_name);
					Print_UG(My_Dirent->d_name);
					Print_File_Size(My_Dirent->d_name);
					Print_Time(My_Dirent->d_name);
					printf(" %-15s", My_Dirent->d_name);
					printf("\n");
				}
			}
		}
	}
}


int main(int argc, char **argv){

	//判断参数个数决定调用函数的情况。函数个数不定，随时补充。
	char ask[4] = {0};
	char route_stree[MAX];
	ask[2] = '\0';
	if(argc == 1){
		
		//如果参数个数为１，打印目前所在文件夹里的文件，不包括隐藏文件，并考虑换行问题。
		Print_No_Paramter(".");
		//否则，判断参数选项
	}else{
		


		for(int i=1; i<argc; i++){
			char Copy_string[MAX];

			//复制参数，过滤需要的字符；
			strcpy(Copy_string, argv[i]);

			//判断命令行参数是否存在路径
			//判断命令行参数是否存在相对路径或者绝对路径
                        //第一个字符为　. 或者　/ 为相对路径和绝对路径，是，拷贝进路径字符串。
			
			if(Copy_string[0] == '.' &&  1 == strlen(Copy_string)){
				strcpy(route_stree, Copy_string);
			}else if((Copy_string[0] == '.' && 1 != strlen(Copy_string)) || Copy_string[0] == '/'){
				strcpy(route_stree,Copy_string);
			}

			//参数写到数组里０
			if(Copy_string[0] == '-' && 1 == strlen(Copy_string)){
				printf("缺少参数\n");
			}else if(Copy_string[0] =='-' && 2 <= strlen(Copy_string)){
				//打印函数接受成功，等待数据处理。
				//printf("%s", Copy_string);
				for(int i=1; i<4; i++){
					if(Copy_string[i] == 'a'){
						ask[0] = Copy_string[i];
					}else if(Copy_string[i] == 'l'){
						ask[1] = Copy_string[i];
					}
				}

				//printf("%c %c", ask[0], ask[1]);

				//失败，准备写循环
				//if(Copy_string[1] == 'l'){
				//	ask[1] = Copy_string[1];
				//}else if(Copy_string[1] == 'a'){
				//	ask[0] = Copy_string[1];
				//}

			}


		}
		
		//Print(route_stree, ask);
	}
	//	Print(route_stree, ask);
	return 0;
}
