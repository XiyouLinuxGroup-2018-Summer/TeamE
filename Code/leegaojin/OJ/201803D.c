#include<stdio.h>
#include<string.h>
int A[10001];
int B[10001]={0};
int main()
{

  int n;
  int i,j;
  int max = 0;
  while(scanf("%d",&n)!=EOF)
  {
    for(i=1;i<n+1;i++)
    {
      scanf("%d",&A[i]);
      B[i] = 1;
    }
    max = 0;
    A[0]=0;
    for(i=1;i<n+1;i++)
    {
      for(j=1;j<i;j++)
      {
        if(A[j]<A[i]&&B[i]<B[j]+1)
          B[i]=B[j]+1;
      }
      if(max < B[i])
        max = B[i];
    }
    printf("%d\n",max);
    memset(B,0,sizeof(B));
    memset(A,0,sizeof(A));
    }
}
