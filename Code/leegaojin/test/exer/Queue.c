#include"Queue.h"
#include<malloc.h>
#include<stdio.h>

Queue *InitQueue()
{
	Queue *pqueue = (Queue *)malloc(sizeof(Queue));
	if(pqueue!=NULL)
	{
		pqueue->front = NULL;
		pqueue->rear = NULL;
		pqueue->size = 0;
	}
	return pqueue;
}

void DestroyQueue(Queue *pqueue)
{
	if(IsEmpty(pqueue)!=1)
		ClearQueue(pqueue);
	free(pqueue);
}

void ClearQueue(Queue *pqueue)
{
	while(IsEmpty(pqueue)!=1)
	{
		DeQueue(pqueue,NULL);
	}

}

int IsEmpty(Queue *pqueue)
{
	if(pqueue->front==NULL&&pqueue->rear==NULL&&pqueue->size==0)
		return 1;
	else
		return 0;
}

int GetSize(Queue *pqueue)
{
	return pqueue->size;
}

PNode GetFront(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->front->data;
	}
	return pqueue->front;
}

PNode GetRear(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->rear->data;
	}
	return pqueue->rear;
}

PNode EnQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		pnode->next = NULL;

		if(IsEmpty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->next = pnode;
		}
		pqueue->rear = pnode;
		pqueue->size++;
	}
	return pnode;
}

PNode DeQueue(Queue *pqueue,Item *pitem)
{
	PNode pnode = pqueue->front;
	if(IsEmpty(pqueue)!=1&&pnode!=NULL)
	{
		if(pitem!=NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		free(pnode);
		if(pqueue->size==0)
			pqueue->rear = NULL;
	}
	return pqueue->front;
}

void QueueTraverse(Queue *pqueue,void (*visit)())
{
	PNode pnode = pqueue->front;
	int i = pqueue->size;
	while(i--)
	{
		visit(pnode->data);
		pnode = pnode->next;
	}

}
