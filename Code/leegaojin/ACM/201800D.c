/*************************************************************************
	> File Name: 201800D.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月05日 星期四 16时37分35秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int n,a,sum=0;
    while(scanf("%d",&n)&&n!=0)
    {
        while(n--)
        {
            scanf("%d",&a);
            sum +=a;
        }
        printf("%d\n",sum);
        sum =0;
    }
    return 0;
}
