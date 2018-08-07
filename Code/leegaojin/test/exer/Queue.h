#ifndef Queue_H
#define Queue_H

typedef int Item;
typedef struct node * PNode;
typedef struct node
{
	Item data;
	PNode next;
}Node;

typedef struct
{
	PNode front;
	PNode rear;
	int size;
}Queue;

Queue *InitQueue();

void DestroyQueue(Queue *pqueue);

void ClearQueue(Queue *pqueue);

int IsEmpty(Queue *pqueue);

int GetSize(Queue *pqueue);

PNode GetFront(Queue *pqueue,Item *pitem);

PNode GetRear(Queue *pqueue,Item *pitem);

PNode EnQueue(Queue *pqueue,Item item);

PNode DeQueue(Queue *pqueue,Item *pitem);

void QueueTraverse(Queue *pqueue,void (*visit)());

#endif
