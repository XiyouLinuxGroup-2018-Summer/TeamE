#include<stdio.h>
int dp[100][100];

int main(int argc, char const *argv[]) {
  int num,line;
  int i,j;
  scanf("%d",&num);
  while(num--)
  {
    scanf("%d",&line);
    for(i=0;i<line;i++)
      for(j=0;j<=i;j++)
      {
        scanf("%d",&dp[i][j]);
      }
    for(i=line-2;i>=0;i--)
     for(j=0;j<=i;j++)
      dp[i][j]=(dp[i][j]+dp[i+1][j])>(dp[i][j]+dp[i+1][j+1])?(dp[i][j]+dp[i+1][j]):(dp[i][j]+dp[i+1][j+1]);

    printf("%d\n",dp[0][0]);
  }
  return 0;
}
