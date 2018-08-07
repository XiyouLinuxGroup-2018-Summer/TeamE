#include<stdio.h>
typedef struct{
  int s,m,n;
  int step;
}queue[100*100*100];

void BFS(int s,int m,int n)
{
  
}

int main()
{
  int s,m,n;
  while(scanf("%d%d%d",&s,&m,&n)==3&&!(!s&&!m&&!n))
  {
    BFS(s,m,n);
  }
}
