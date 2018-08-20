#include<stdio.h>
#include<mysql/mysql.h>
#include<string.h>
int main(){
	MYSQL mysql;
	//初始化MYSQL句柄
	if(NULL == mysql_init(&mysql)){
		printf("init: %s\n",mysql_error(&mysql));
	}

	//建立MYSQL连接

	if(NULL == mysql_real_connect(&mysql,"localhost","debian-sys-maint","ZCoY8eh3ichrX4X0","client_number",0,NULL,0)){
		printf("connect: %s\n",mysql_error(&mysql));
	}

	//执行插入语句
	char* sql=NULL;
	sql = (char*)calloc(1024,sizeof(char));
	//= "INSERT INTO c(name,passwd) VALUES('lil','1232')";
	char name[20];
	char passwd[20];
	printf("请输入用户名：");
	scanf("%s",name);
	printf("请输入密码：");
	scanf("%s",passwd);
  sprintf(sql,"INSERT INTO c(name,passwd) VALUES('%s','%s')",name,passwd);
	if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
		printf("query: %s\n",mysql_error(&mysql));
	}
/*  //执行删除语句
	sql = "DELETE FROM c WHERE name='lil'";
	if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
		printf("query: %s\n",mysql_error(&mysql));
	}
*/
	//执行查询语句
	sql = "SELECT id,name,passwd FROM c";
	if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
		printf("query: %s\n",mysql_error(&mysql));
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res))) {
		printf("%s %s %s\n",row[0],row[1],row[2]);
	}
	mysql_free_result(res);
	mysql_close(&mysql);
	free(sql);    //释放sql
}
