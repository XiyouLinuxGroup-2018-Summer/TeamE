/*************************************************************************
	> File Name: 201800H.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 11时29分12秒
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

