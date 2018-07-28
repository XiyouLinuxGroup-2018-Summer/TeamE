/*************************************************************************
	> File Name: C1.c
	> Author:
	> Mail:
	> Created Time: 2018年07月23日 星期一 14时38分47秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    char a[100000];
    int flag = 1;
    int n,i,lenth;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%s",&a);
        lenth=strlen(a);
        lenth--;
        for(i=0;i<lenth;i++,lenth--)
        {
            if(a[i]!=a[lenth])
            {
            flag=0;
            break;
            }
        }
        if(flag)
            printf("yes\n");
        else if(!flag)
            printf("no\n");
        flag=1;
    }
    return 0;
}
