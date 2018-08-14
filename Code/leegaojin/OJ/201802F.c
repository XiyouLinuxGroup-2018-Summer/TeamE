#include<stdio.h>
#include<string.h>
int step_x[8]={0,1,1,1,0,-1,-1,-1};
int step_y[8]={1,1,0,-1,-1,-1,0,1};
char oilm[100][100];
int visit[100][100];
int m,n;
/*void print_m(int m,int n)
{
  int i,j;
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
     printf("%c ",oilm[i][j]);
     printf("\n");
  }
}*/
void DFS(int i,int j)
{
   if(i<0||i>=m||j<0||j>=n||visit[i][j]==1||oilm[i][j]=='*')
   return;
   visit[i][j]=1;
   int k;
   for(k=0;k<8;k++)
   {
     DFS(i+step_x[k],j+step_y[k]);
   }
}
void init()
{
  int i,j;
  int count=0;
  memset(oilm,0,sizeof(oilm));
  memset(visit,0,sizeof(visit));
  for(i=0;i<m;i++)
  {
    scanf("%s",&oilm[i]);
  }
  for(i=0;i<m;i++)
   for(j=0;j<n;j++)
     if(visit[i][j]==0&&oilm[i][j]=='@'){
       count++;
       DFS(i,j);
     }
  printf("%d\n",count);

  //print_m(m,n);

}

int main()
{

  while(scanf("%d %d",&m,&n)==2&&m&&n)
  {
    init();
  }
}
