/*************************************************************************
	> File Name: 201801M.c
	> Author:
	> Mail:
	> Created Time: 2018年07月24日 星期二 13时59分24秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    char a[1000];
    int lenth,i;
    while(fgets(a,sizeof(a),stdin)!=NULL)
    {
        lenth=strlen(a);
        for(i=0;i<lenth;i++)
        {
            if(a[i]==' '&&a[i+1]!=' ')
            a[i+1]=a[i+1]-'a'+'A';
        }
        a[0]=a[0]-'a'+'A';
        for(i=0;i<lenth;i++)
        {
            putchar(a[i]);
        }
    }
    return 0;
}
