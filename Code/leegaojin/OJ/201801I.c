/*************************************************************************
	> File Name: 201801I.c
	> Author:
	> Mail:
	> Created Time: 2018年07月24日 星期二 10时55分14秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    float a,b,c;
    int n;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%f%f%f",&a,&b,&c);
        if(a+b>c&&a+c>b&&b+c>a&&!(a<=0||b<=0||c<=0))
        printf("YES\n");
        else
        printf("NO\n");
    }
    return 0;
}
