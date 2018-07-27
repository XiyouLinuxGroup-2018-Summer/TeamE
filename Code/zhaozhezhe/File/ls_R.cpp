#include <memory>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <cerrno>
#include <sys/sysmacros.h>
#include <string>

#define MAX 1000
#define N  1024

using namespace std;
void Print(char *route, int *value);
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
//
#if 0
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
#endif
void Print(char *route, int *value){
	

	printf("将要读取的为：\n");
	DIR *My_File_DIR;
	struct dirent *My_Dirent;

	//struct stat My_Stat;
	//参数为空，打印程序所在目录下非隐藏文件。
	
	//打开文件
	My_File_DIR = opendir(route);
	//这里调用会缺失一个文件
	//My_Dirent = readdir(My_File_DIR)
	
	//value[0] 为查看隐藏文件的权限 value[1]为查看文件的详细内容;
	
	if(value[0] == 0 && value[1] == 0){
		printf("将要读取的为：- - - -- - -- \n");
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
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....
	}
       	if(value[0] == 1 && value[1] == 0){
		printf("将要读取的为：-a \n");
		int n = 0;
		while((My_Dirent = readdir(My_File_DIR))!= NULL){
			printf(" %-15s", My_Dirent->d_name);
			n++;
			if(n%7==0){
				printf("\n");
			}
	
		}
	}
	if(value[0] == 1 && value[1] == 1){
		printf("将要读取的为：-la\n");

		while((My_Dirent = readdir(My_File_DIR)) != NULL){
			Print_Jur(My_Dirent->d_name);
			Print_Link(My_Dirent->d_name);
			Print_UG(My_Dirent->d_name);
			Print_File_Size(My_Dirent->d_name);
			Print_Time(My_Dirent->d_name);
			printf(" %-15s", My_Dirent->d_name);
			printf("\n");
		}
	
	}
	if(value[0] == 0 && value[1] == 1){
		printf("将要读取的为：-l\n");
		while((My_Dirent = readdir(My_File_DIR)) != NULL){
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

	closedir(My_File_DIR);
}

int Print_R(char *route, int *value){
	//创建动态数组，去保存每一个读进来的文件。
	//一个指针去指着。
	
	vector<string>  My_Filename;

	printf("将要读取的路径为：%s \n", route);
	DIR *My_File_DIR;
	struct dirent *My_Dirent;
	//struct stat My_stat;

	//打开文件
	My_File_DIR = opendir(route);
	
	
	//判断文件类型。
	//if (S_ISDIR(My_Stat.st_mode)){
        //        Create(Move_Filename, )
	//} 
	
	
	if(value[0] == 0 && value[1] == 0){
		int n = 0;
		while((My_Dirent = readdir(My_File_DIR))!= NULL){
			struct stat My_stat;
			stat(My_Dirent->d_name, &My_stat);
			if(S_ISDIR(My_stat.st_mode)){
				//保存文件名
				My_Filename.push_back(My_Dirent->d_name);
			}
			if (S_ISLNK(My_stat.st_mode)){
                		;
        		}
		       	if (S_ISREG(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISCHR(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISBLK(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISFIFO(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISSOCK(My_stat.st_mode)){
        		        ;
        		}
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
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....
		
		
		vector<string>::iterator it1 = My_Filename.begin();
		int len = 0;
		while(it1 != My_Filename.end()){
			it1++;
			len++;
			//cout << len <<endl;
		}

		if(len == 2){
			cout << 3 << endl;
			return 0;
		}

		cout << 4 << endl;
	
		char old_pwd[500];
		getcwd(old_pwd, 500);
		while(!My_Filename.empty()){
		string name = My_Filename.back();
                My_Filename.pop_back();
		
                string::iterator str = name.begin();
		char Name[name.length()+1];
		
	       	for(int i=0; i<name.length(); i++){
	       		Name[i] = name.at(i);
	       	}
		Name[name.length()] = 0;
               	if( *str == '.'){
                        //cout << "隐藏文件" <<endl;
               	}else{

			//获得当前工作目录，
			char pwd[1000];
			
			strcpy(pwd, old_pwd);

			//char old_pwd[strlen(pwd)];
			//strcpy(old_pwd, pwd);
			//连接工作目录，
			if(pwd[strlen(pwd)-1] != '/'){
				strcat(pwd,"/");
			}
			strcat(pwd, Name);

			//进入工作目录。
			if(chdir(pwd) == -1){
				return -1;
				exit(0);
			}else{

				Print_R(pwd, value);
			}
				/*
				if(flag == -1){

					if(old_pwd[strlen(pwd)-1] != '/'){
                                		strcat(pwd,"/");
                        		}
                        		strcat(old_pwd, Name);
					if(chdir(old_pwd) == -1){
						return -1;
					}else{
						Print_R(old_pwd, value);
					}
				}
				*/
			}
		}
			//切换工作目录，
			//Print_R()调用。^

	closedir(My_File_DIR);
	}
#if 0
			
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

#endif		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....


	if(value[0] == 1 && value[1] == 0){
		printf("将要读取的为：-aR \n");
		int n = 0;
		while((My_Dirent = readdir(My_File_DIR))!= NULL){
		
			struct stat My_stat;
			stat(My_Dirent->d_name, &My_stat);
			if(S_ISDIR(My_stat.st_mode)){
				//保存文件名
				My_Filename.push_back(My_Dirent->d_name);
			}
			if (S_ISLNK(My_stat.st_mode)){
                		;
        		}
		       	if (S_ISREG(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISCHR(My_stat.st_mode)){
        		        ;
			}
			if (S_ISBLK(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISFIFO(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISSOCK(My_stat.st_mode)){
        		        ;
        		}
			printf(" %-15s", My_Dirent->d_name);
			n++;
			if(n%7==0){
				printf("\n");
			}
	
		}
		
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....
		
		
		vector<string>::iterator it1 = My_Filename.begin();
		int len = 0;
		while(it1 != My_Filename.end()){
			it1++;
			len++;
			//cout << len <<endl;
		}

		if(len == 2){
			cout << 3 << endl;
			return 0;
		}

		cout << 4 << endl;
	
		char old_pwd[500];
		getcwd(old_pwd, 500);
		while(!My_Filename.empty()){
		string name = My_Filename.back();
                My_Filename.pop_back();
		
                string::iterator str = name.begin();
		char Name[name.length()+1];
		
	       	for(int i=0; i<name.length(); i++){
	       		Name[i] = name.at(i);
	       	}
		Name[name.length()] = 0;
               	if( *str == '.'){
                        //cout << "隐藏文件" <<endl;
               	}else{

			//获得当前工作目录，
			char pwd[1000];
			
			strcpy(pwd, old_pwd);

			//char old_pwd[strlen(pwd)];
			//strcpy(old_pwd, pwd);
			//连接工作目录，
			if(pwd[strlen(pwd)-1] != '/'){
				strcat(pwd,"/");
			}
			strcat(pwd, Name);

			//进入工作目录。
			if(chdir(pwd) == -1){
				return -1;
				exit(0);
			}else{

				Print_R(pwd, value);
			}
				
		}
		
		}
	}

	if(value[0] == 1 && value[1] == 1){
		printf("将要读取的为：-laR\n");

		while((My_Dirent = readdir(My_File_DIR)) != NULL){
			struct stat My_stat;
			stat(My_Dirent->d_name, &My_stat);
			if(S_ISDIR(My_stat.st_mode)){
				//保存文件名
				My_Filename.push_back(My_Dirent->d_name);
			}
			if (S_ISLNK(My_stat.st_mode)){
                		;
        		}
		       	if (S_ISREG(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISCHR(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISBLK(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISFIFO(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISSOCK(My_stat.st_mode)){
        		        ;
        		}
			//碰到隐藏文件不操作。
			
				
				Print_Jur(My_Dirent->d_name);
				Print_Link(My_Dirent->d_name);
				Print_UG(My_Dirent->d_name);
				Print_File_Size(My_Dirent->d_name);
				Print_Time(My_Dirent->d_name);
				printf(" %-15s", My_Dirent->d_name);
				printf("\n");
		}
		
		printf("\n");
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....
		
		
		vector<string>::iterator it1 = My_Filename.begin();
		int len = 0;
		while(it1 != My_Filename.end()){
			it1++;
			len++;
			//cout << len <<endl;
		}

		if(len == 2){
			cout << 3 << endl;
			return 0;
		}

		cout << 4 << endl;
	
		char old_pwd[500];
		getcwd(old_pwd, 500);
		while(!My_Filename.empty()){
		string name = My_Filename.back();
                My_Filename.pop_back();
		
                string::iterator str = name.begin();
		char Name[name.length()+1];
		
	       	for(int i=0; i<name.length(); i++){
	       		Name[i] = name.at(i);
	       	}
		Name[name.length()] = 0;
               	if( *str == '.'){
                        //cout << "隐藏文件" <<endl;
               	}else{

			//获得当前工作目录，
			char pwd[1000];
			
			strcpy(pwd, old_pwd);

			//char old_pwd[strlen(pwd)];
			//strcpy(old_pwd, pwd);
			//连接工作目录，
			if(pwd[strlen(pwd)-1] != '/'){
				strcat(pwd,"/");
			}
			strcat(pwd, Name);

			//进入工作目录。
			if(chdir(pwd) == -1){
				return -1;
				exit(0);
			}else{

				Print_R(pwd, value);
			}
		}
		}
	}

	if(value[0] == 0 && value[1] == 1){
		printf("将要读取的为：-lR\n");
		while((My_Dirent = readdir(My_File_DIR)) != NULL){
			//碰到隐藏文件不操作。
			struct stat My_stat;
			stat(My_Dirent->d_name, &My_stat);
			if(S_ISDIR(My_stat.st_mode)){
				//保存文件名
				My_Filename.push_back(My_Dirent->d_name);
			}
			if (S_ISLNK(My_stat.st_mode)){
                		;
        		}
		       	if (S_ISREG(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISCHR(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISBLK(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISFIFO(My_stat.st_mode)){
        		        ;
        		}
			if (S_ISSOCK(My_stat.st_mode)){
        		        ;
        		}
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
		
		printf("\n");
		//判断参数。打印参数是否可以正确的打印出来....
		//可以接受单个命令，可以接受多个。处理函数....
		
		
		vector<string>::iterator it1 = My_Filename.begin();
		int len = 0;
		while(it1 != My_Filename.end()){
			it1++;
			len++;
			//cout << len <<endl;
		}

		if(len == 2){
			cout << 3 << endl;
			return 0;
		}

		cout << 4 << endl;
	
		char old_pwd[500];
		getcwd(old_pwd, 500);
		while(!My_Filename.empty()){
		string name = My_Filename.back();
                My_Filename.pop_back();
		
                string::iterator str = name.begin();
		char Name[name.length()+1];
		
	       	for(int i=0; i<name.length(); i++){
	       		Name[i] = name.at(i);
	       	}
		Name[name.length()] = 0;
               	if( *str == '.'){
                        //cout << "隐藏文件" <<endl;
               	}else{

			//获得当前工作目录，
			char pwd[1000];
			
			strcpy(pwd, old_pwd);

			//char old_pwd[strlen(pwd)];
			//strcpy(old_pwd, pwd);
			//连接工作目录，
			if(pwd[strlen(pwd)-1] != '/'){
				strcat(pwd,"/");
			}
			strcat(pwd, Name);

			//进入工作目录。
			if(chdir(pwd) == -1){
				return -1;
				exit(0);
			}else{

				Print_R(pwd, value);
			}
				
		}
	}
	
	}

	return 0;
}

int main(int argc, char **argv){
	
	//判断参数个数决定调用函数的情况。函数个数不定，随时补充。
	int ask[3] = {0};
	char route_stree[MAX];
	int flag = 0;
	
	if(argc == 1){
		
		//如果参数个数为１，打印目前所在文件夹里的文件，不包括隐藏文件，并考虑换行问题。
		strcpy(route_stree, ".");
		//否则，判断参数选项
	}else{
		
		
		
		for(int i=1; i<argc; i++){
			char Copy_string[MAX];
		
			//复制参数，过滤需要的字符；
			strcpy(Copy_string, argv[i]);

			//判断命令行参数是否存在路径
			//判断命令行参数是否存在相对路径或者绝对路径
                        //第一个字符为　. 或者　/ 为相对路径和绝对路径，是，拷贝进路径字符串。
			

			
			//if(Copy_string[0] != '-'){
				//不管在哪里工作，都设置工作目录为参数的路径

			//	if(chdir(Copy_string) < 0){
			//		perror("chdir ");
			//	}
			//	strcpy(route_stree, Copy_string);
			//	flag = 1;
			//}
			
			if(Copy_string[0] == '.' &&  1 == strlen(Copy_string)){
				strcpy(route_stree, Copy_string);
				flag = 1;
			}else if( Copy_string[1] == '.' && 1 != strlen(Copy_string)){
					
				strcpy(route_stree, Copy_string);
		 		flag = 1;		
				
			}else if(Copy_string[0] == '/'){
				
				if(chdir(Copy_string) == -1){
					printf("No this pathname.\n");
				}else{
					strcpy(route_stree,Copy_string);
					flag = 1;
				}
			}
			
			//参数写到数组里０
			if((Copy_string[0] == '-' && 1 == strlen(Copy_string))){
				printf("缺少参数,或参数过多！\n");
				exit(-1);
			}else if(Copy_string[0] =='-' && (2 ==  strlen(Copy_string) || 3 == strlen(Copy_string) || 4 == strlen(Copy_string))){
				//打印函数接受成功，等待数据处理。
				//printf("%s", Copy_string);
			
				for(int i=1; i<strlen(Copy_string); i++){
					
					if(Copy_string[i] == 'a'){
						ask[0] = 1;
					}
					if(Copy_string[i] == 'l'){
						ask[1] = 1;
					}
				       	if(Copy_string[i] == 'R'){
						ask[2] = 1;
					}
					
					if(Copy_string[i] != 'a' && Copy_string[i] != 'l' &&  Copy_string[i] != 'R'){
						printf("请输入正确的参数。\n");
						exit(0);
					}


				}
		
			}
		}	
		
		if(!flag){
			//没有写路径，打开当前文件夹
			strcpy(route_stree,".");
		}
		
		
		//Print(route_stree, ask);
	}
	if(ask[2] == 1){
		Print_R(route_stree, ask);
	}else{
		Print(route_stree, ask);
	}
	return 0;
}
