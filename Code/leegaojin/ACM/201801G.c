/*************************************************************************
	> File Name: 201801G.c
	> Author:
	> Mail:
	> Created Time: 2018年07月24日 星期二 10时04分20秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int m,n,j,i;
    int a[1000]={0};
    while(scanf("%d %d",&m,&n)&&!(m==0&&n==0))
    {
        for(i=0;i<m;i++)
        {
            scanf("%d",&a[i]);
        }
        for(i=0;i<m;i++)
        {
            if(n<a[i])
            {
                for(j=m;j>i;j--)
                {
                    a[j]=a[j-1];
                }
                a[i]=n;
            }
        }
        for(i=0;i<=m;i++)
        {
            printf("%d",a[i]);
            if(i < m)
			{
				printf(" ");
			}
        }
        printf("\n");
    }
}
