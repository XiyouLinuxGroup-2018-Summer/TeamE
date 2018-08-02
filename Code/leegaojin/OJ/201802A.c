#include<stdio.h>
int maze[5][5];
int visit[5][5];
int step[4][2]={(-1,0),(1,0),(0,-1),(0,1)};
struct node{
  int x,y,pre;
}queue[100];
int main()
{


}
void maze()
{
  int i,j;
  for(i=0;i<5;i++)
  {
    for(j=0;j<5;j++)
    {
      scanf("%d",&maze[i][j]);
    }
  }
}
void BFS(int x,int y)
{
  maze();/////初始化迷宫
  int front=0;
  int rear =1;

}
