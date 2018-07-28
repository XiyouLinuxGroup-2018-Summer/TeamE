/*************************************************************************
	> File Name: B1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月22日 星期日 19时43分48秒
 ************************************************************************/

#include<stdio.h>
#define PI 3.1415927
int main()
{
    double a,b;
    while(scanf("%lf",&a)!=EOF)
    {   
        b=(4.0/3.0)*PI*a*a*a;
        printf("%.3lf\n",b);
    }
    return 0;
}
