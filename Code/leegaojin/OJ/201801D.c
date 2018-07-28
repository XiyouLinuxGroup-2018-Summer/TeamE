/*************************************************************************
	> File Name: D1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 16时51分23秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int a,b,c,m,n;
    while(scanf("%d %d",&a,&b)!=EOF)
    {
        m=a;
        n=b;
        while(b!=0)
        {
            c=a%b;
            a=b;
            b=c;
        }
        printf("%d\n",m*n/a);
    }
    return 0;
}
