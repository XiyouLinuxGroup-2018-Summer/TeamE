/*************************************************************************
	> File Name: 1.c
	> Author:
	> Mail:
	> Created Time: 2018年07月28日 星期六 16时14分22秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tag_unimportant {
    char *name;
    int num;
} A;
void func(A *a)
{
    a->name = (char*)malloc(sizeof(char)*30);
    strcpy(a->name, "Xiyou Linux Group");
    a->num = 20180728;
}
int main(int argc, char *argv[])
{
    A *a;
    a = (A *)malloc(sizeof(A));
    func(a);
    printf("%s %d\n", a->name, a->num);
    return 0;
}
