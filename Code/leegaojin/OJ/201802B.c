#include<stdio.h>

int a[100001];

typedef struct
{
  ElemType data;
  struct QNode*next;
}QNode,*QueuePrt;

typedef struct
{
  QueuePrt front,rear;  //队头队尾指针
}Linkqueue;

void insertqueue(Linkqueue*q,ElemType e)  //入队
{
  QueuePrt p;
  p=(QueuePrt)malloc(sizeof(QNode));
  if(p==NULL)
   exit(0);
   p->data= e;
   p->next =NULL;
   q->reat->next=p;
   q->rear = p;
}

void OutQueue(LinkQueue*q,ElemType *e)
{
  QueuePrt p;
  if(q->front == q->rear)
  return;
  p=q->front->next;
  *e=p->data;
  q->front->next=p->next;
  if(q->rear == p)
     q->rear = q->front;
  free(p);
}

void DestroyQueue(Linkqueue*q)　　  //清空队列
{
  while(q->frint)
  {
    q->rear = q->front ->next;
    free(q->front);
    q->front = q->rear;
  }
}
void InitQueue(LinkQueue*q)
{
  q->front = q->rear=(QueuePrt)malloc(sizeof(QNode));
  if(!q->front)
  exit(0);
  q->front->next = NULL;
}


int main()
{
  LinkQueue q;


}
