/*************************************************************************
	> File Name: 201801K.c
	> Author:
	> Mail:
	> Created Time: 2018年07月24日 星期二 14时46分32秒
 ************************************************************************/

#include<stdio.h>
int sushu(int);
int main()
{
    int m,n,i,j,num;
    int flag = 1;
    while(scanf("%d%d",&m,&n)!=EOF&&!(m==0&&n==0))
    {
        if(m<-39||n>50)
        {
            printf("Sorry\n");
            continue;
        }
        for(i=m;i<=n;i++)
        {
            num = i*i+i+41;
            if(sushu(num))
                continue;
            else
            {
                flag = 0;
                break;
            }
        }
        if(flag)
        printf("OK\n");
        else
        printf("Sorry\n");
    }
    return 0;
}
int sushu(int a)
{
    int i;
    int flag=1;
    
    for(i=2;i<a;i++)
    {
        if(a%i==0)
        {
        flag=0;
        break;
        }
    }
    if(flag)
    return 1;
    else if(!flag)
    return 0;
}
