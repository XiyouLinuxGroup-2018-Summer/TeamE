/*************************************************************************
	> File Name: E1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月23日 星期一 17时08分33秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int n;
    int a,b;
    char c;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%s %d %d",&c,&a,&b);
        if(c=='+')
            {
                printf("%d\n",a+b);
            }
        else if(c=='-')
            {
                printf("%d\n",a-b);
            }
        else if(c=='*')
        {
            printf("%d\n",a*b);
        }
        else if(c=='/')
        {
            if(a%b==0)
            {
                printf("%d\n",a/b);
            }
            else if(a%b!=0)
            {
                printf("%.2f\n",(float)a/b);
            }
        }

    }
    return 0;
}
