/*************************************************************************
	> File Name: 201801J.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 12时49分58秒
 ************************************************************************/

#include<stdio.h>
int qinhe(int,int);
int main()
{
    int num,a,b;
    int m,n;
    scanf("%d",&num);
    while(num--)
    {
        scanf("%d%d",&a,&b);
        n=qinhe(a,b);
        if(n)
        printf("YES\n");
        else
        printf("NO\n");
    }
}
int qinhe(int a,int b)
{
    int i,num1=0,num2=0;
    for(i=1;i<a;i++)
    {
        if(a%i==0)
        {
            num1+=i;
        }
    }
    for(i=1;i<b;i++)
    {
        if(b%i==0)
        {
            num2+=i;
        }
    }
    if(num1==b&&num2==a)
    return 1;
    else
    return 0;
}
