#include<stdio.h>
#include<string.h>
struct node{
  int x;
  int step;
}queue[100001];
int visit[100001]={0};
int inter(int a)
{
  if(a>=0&&a<=100000&&!visit[a])
  return 1;
  else
  return 0;

}
void BFS(int a,int b)
{
//  int visit[100001]={0};
  memset(queue,0,sizeof(queue));
  memset(visit,0,sizeof(visit));
  visit[a]=1;
  int front = 0;
  int rear =1;
  int next_step =0;
  int i;
  queue[0].x=a;
  queue[0].step = 0;
  while(front<rear)
  {
    if(queue[front].x==b)
    {
      printf("%d\n",queue[front].step);
      return;
    }
    for(i=0;i<3;i++){
      if (i == 0)
        next_step=queue[front].x+1;
      else if(i==1)
        next_step=queue[front].x-1;
      else
        next_step=queue[front].x*2;
      if(inter(next_step))
      {
      visit[next_step]=1;
      queue[rear].x=next_step;
      queue[rear].step = queue[front].step+1;
      rear++;
      }

    }
      front ++;
  }

}
int main()
{
  int a,b;
  while(scanf("%d %d",&a,&b)!=EOF)
  {
    //visit[100001]={0};

    BFS(a,b);
  }
  return 0;
}
