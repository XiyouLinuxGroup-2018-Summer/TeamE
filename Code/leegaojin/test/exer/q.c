#include"Queue.h"
#include<stdio.h>

void print(Item i)
{
	printf("该节点元素为%d\n",i);
}

int main()
{
	Queue *pq = InitQueue();
	int i,item;
	printf("0-9依次入队并输出如下：\n");
	for(i=0;i<10;i++)
	{
		EnQueue(pq,i);
		GetRear(pq,&item);
		printf("%d ",item);
	}

	printf("\n从队头到队尾遍历并对每个元素执行print函数：\n");
	QueueTraverse(pq,print);

	printf("队列中元素依次出队列并输出如下：\n");
	for(i=0;i<10;i++)
	{
		DeQueue(pq,&item);
		printf("%d ",item);
	}
	ClearQueue(pq);
	if(IsEmpty(pq))
		printf("\n将队列置空成功\n");
	DestroyQueue(pq);
	printf("队列已被销毁\n");
}
