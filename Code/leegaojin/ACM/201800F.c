/*************************************************************************
	> File Name: 201800F.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 11时16分58秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int n,a,sum=0;
    while(scanf("%d",&n)!=EOF)
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
