/*************************************************************************
	> File Name: 201800E.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月05日 星期四 17时04分02秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int n,num,a,sum;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%d",&num);
                sum=0;
        while(num--)
        {
            scanf("%d",&a);
            sum+=a;
        }
        if(n!=0)
        printf("%d\n\n",sum);
        else
        printf("%d\n",sum);
    }
    return 0;
}
