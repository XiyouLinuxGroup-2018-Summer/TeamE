/*************************************************************************
	> File Name: 201801F.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 09时12分26秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#define MAX 1000
int main()
{
    char a[MAX];
    int n,lenth,i,num;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%s",a);
        lenth=strlen(a);
        for(i=0;i<lenth;i++)
        {
            if(a[i]>=48&&a[i]<=58)
            num++;
        }
        printf("%d\n",num);
        num = 0;
    }
    return 0;
}
