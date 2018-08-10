#include<stdio.h>
int A[10001];
int B[10001];
int main()
{

  int n;
  int i,j;
  int max = 0;
  while(scanf("%d",&n)&&n!=0)
  {
    for(i=0;i<n;i++)
    {
      scanf("%d",&A[i]);
      B[i]=A[i];
    }
    max = 0;

    for(i=0;i<n;i++)
    {
      for(j=0;j<i;j++)
      {
        if(A[j]<A[i]&&B[i]<B[j]+A[i])
         B[i]=B[j]+A[i];
      }
      if(max<B[i])
       max = B[i];
    }
      printf("%d\n",max);
    }
}
