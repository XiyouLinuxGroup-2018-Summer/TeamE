/*************************************************************************
	> File Name: A1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月22日 星期日 17时39分52秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    char a,b,c;
    while(scanf("%c%c%c",&a,&b,&c)!=EOF)
    {
        getchar();
        
        if(a<b&&b<c)
            printf("%c %c %c\n",a,b,c);
        else if(a<c&&c<b)
            printf("%c %c %c\n",a,c,b);
        else if(b<a&&a<c)
            printf("%c %c %c\n",b,a,c);
        else if(b<c&&c<a)
            printf("%c %c %c\n",b,c,a);
        else if(c<a&&a<b)
            printf("%c %c %c\n",c,a,b);
        else if(c<b&&b<a)
            printf("%c %c %c\n",c,b,a);
    }
    return 0;
}
