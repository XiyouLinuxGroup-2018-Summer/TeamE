#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	
	int mod;
	int mod_u;
	int mod_g;
	int mod_o;
	int mod_8;
	//参数过少不能进行判断。
	if(argc < 3)
	{
		printf("参数少于三个！\n");
		exit(0);
	}

	//第二个参数必须为权限值。
	mod = atoi(argv[1]);
	//权限值必须在范围内
	if(mod > 777 || mod < 0)
	{
		printf("权限值错误\n");
		exit(0);
	}
	mod_u = mod / 100;
	mod_g = (mod-mod_u*100)/10;
	mod_o = mod - mod_u*100 - mod_g*10;
 
	mod_8 = (mod_u * 8 * 8) + (mod_g *8) + mod_o;

	if(!chmod(argv[2],mod_8)){
		printf("错误\n");
	}
	else
		printf("正确\n");
	return 0;
}
