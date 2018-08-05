#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "iostream"
#include <queue>
using namespace std; 
int f;
int l;
//基本操作：
//
//push(x) 将x压入队列的末端
//
//pop() 弹出队列的第一个元素(队顶元素)，注意此函数并不返回任何值
//
//front() 返回第一个元素(队顶元素)
//
//back() 返回最后被压入的元素(队尾元素)
//
//empty() 当队列为空时，返回true
//
//size() 返回队列的长度
//
// 
//
//使用方法：
//
//头文件：
//
//#include <queue>
// 声明方法：
//
//1、普通声明
//
//queue<int>q;
//2、结构体
//
//struct node
//{    
//   int x, y;
//};
//queue<node>q;
// 
int n[110000];		//记录走过没走过顺便记录下走了多少 
struct node
{
	int x;
};
queue <node> q;
int bfs()
{ 
	memset(n, 0, sizeof(n));
	while(!q.empty())	//每次来之前先清空下 
	{
		q.pop();
	}
	node m;
	m.x = l;
	n[l] = 1;
	q.push(m);
	while(!q.empty())
	{
		node a = q.front();
//		if(a.x == f)
//		return n[f];
		if(n[f])		//如果有，就退出 
		return n[f];
		q.pop();
		if(a.x < 0 || a.x > 110000)
		continue; 
//	if((n[a.x + 1] == 0)&& ((a.x + 1) <= 110000))
	if(((a.x + 1) <= 110000) && (n[a.x + 1] == 0))
	{
		node next;
		n[a.x + 1] = n[a.x] + 1;
		next.x = a.x + 1;
		q.push(next);
	}
	if((n[a.x - 1] == 0)&& ((a.x - 1) >= 0))
	{
		node next;
		n[a.x - 1] = n[a.x] + 1;
		next.x = a.x - 1;
		q.push(next);
	}
	//if((n[a.x * 2] == 0 )&& ((a.x * 2) < 110000))
	if(((a.x * 2) < 110000) && (n[a.x * 2] == 0 ))
	{
		node next;
		n[a.x * 2] = n[a.x] + 1;
		next.x = a.x * 2;
		q.push(next);
	}
	}
}
int main()
{
	int a,b;
	while(scanf("%d%d",&a,&b) != EOF)
	{
		l = a;
		f = b;
		printf("%d\n",bfs() - 1);
	}
	return 0; 
}
