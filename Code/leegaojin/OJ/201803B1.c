#include<stdio.h>
#include<string.h>
int dp[32800];
int main()
{
    int n,i,j;
    memset(dp,0,sizeof(dp));
    dp[0]=1;
    for(i=1;i<=3;i++)
    {
        for(j=i;j<=32770;j++)
        {
            dp[j]+=dp[j-1];
        }
     }
     while(scanf("%d",&n)==1)
     {
          printf("%d\n",dp[n]);
     }
     return 0;
}
