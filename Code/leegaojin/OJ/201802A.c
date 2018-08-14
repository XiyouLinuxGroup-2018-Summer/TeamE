#include<stdio.h>
//int maze[5][5]={0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0};
int maze[5][5];
int visit[5][5]={0};
int step_x[4]={1,0,-1,0};
int step_y[4]={0,1,-1,0};
struct node{
  int x,y,pre;
}queue[100];
int next_x,next_y;
void maze_p(void)
{
  int i,j;
  for(i=0;i<5;i++)
    for(j=0;j<5;j++)
      scanf("%d",&maze[i][j]);
}
int inter(int x,int y)
{
  if(x>=0&&x<=4&&y>=0&&y<=4&&maze[x][y]==0)
    return 1;
  else
  {

    return 0;
  }
}
void output(int a)
{
  if(queue[a].pre!=-1)
  {
    output(queue[a].pre);
    printf("(%d, %d)\n",queue[a].x,queue[a].y);
  }
}
void print_maze()
{
  int i,j;
  for(i=0;i<5;i++){
   for(j=0;j<5;j++)
    printf("%d ",maze[i][j]);
    printf("\n");
  }
}
void BFS()
{
  maze_p();/////初始化迷宫
  //print_maze();
  int front=0;
  int rear =1;
  int i;
  queue[0].x=0;
  queue[0].y=0;
  queue[0].pre=-1;
  visit[0][0]=1;
  while(front<rear)
  {
    for(i=0;i<4;i++)
    {
      next_x=queue[front].x+step_x[i];
      next_y=queue[front].y+step_y[i];
      if(inter(next_x,next_y)&&visit[next_x][next_y]==0)
      {
        visit[next_x][next_y]=1;
        queue[rear].x=next_x;
        queue[rear].y=next_y;
        queue[rear].pre = front;
        rear ++;
      }
      if(next_x==4&&next_y==4)
      {
        printf("(0, 0)\n");
        output(rear-1);
        return ;
      }
    }
    front++;
  }
}
int main()
{
     BFS();
     return 0;

}
