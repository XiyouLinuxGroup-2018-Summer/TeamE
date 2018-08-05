#include "iostream"
#include "cmath"
#include "cstdio"
#include "queue"
#include "cstdlib"
#include "cstring"

struct pingzi
{
	int s,n,m;
};
pingzi sf;
using namespace std;
queue <pingzi> p;
int step[105][100][100] = { 0 };

int bfs()
{
	memset(step,0,sizeof(step));		//初始化步数和清空队列 
	while(!p.empty())
		p.pop();
	step[sf.s][0][0] = 1;				//把第一次的扔进去 
	pingzi cc;
	cc.m = 0;
	cc.n = 0;
	cc.s = sf.s;
	p.push(cc);
	while(!p.empty())
	{
		pingzi a = p.front();
	//	printf("error!!! a.s = %d,a.m = %d,a.n = %d\n",a.s,a.m,a.n);
		if((a.s + a.m + a.n) != sf.s)
		{
			printf("error!!! a.s = %d,a.m = %d,a.n = %d\n",a.s,a.m,a.n);
		} 
		if(step[sf.s / 2][sf.s / 2][0] || step[sf.s / 2][0][sf.s / 2])
		return (step[sf.s / 2][sf.s / 2][0] == 0) ? (step[sf.s / 2][0][sf.s / 2]):(step[sf.s / 2][sf.s / 2][0]); 
		p.pop();
		if(a.m < sf.m && a.s)		//s给m倒 
		{
			pingzi next = a;
			int b =  sf.m - next.m;
			if(b <= a.s)		//当b小于a.s时
			{
				next.s -= b;
				next.m = sf.m; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.m += next.s;
				next.s = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}
		if(a.n < sf.n && a.s)		//s给n倒 
		{
			pingzi next = a;
			int b =  sf.n - next.n;
			if(b <= a.s)		//当b小于a.s时
			{
				next.s -= b;
				next.n = sf.n; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.n += next.s;
				next.s = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}

		if(a.m < sf.m && a.n)		//n给m倒 
		{
			pingzi next = a;
			int b =  sf.m - next.m;
			if(b <= a.n)		//当b小于a.s时
			{
				next.n -= b;
				next.m = sf.m; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.m += next.n;
				next.n = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}
		
				if(a.n < sf.n && a.m)		//m给n倒 
		{
			pingzi next = a;
			int b =  sf.n - next.n;
			if(b <= a.m)		//当b小于a.s时
			{
				next.m -= b;
				next.n = sf.n; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.n += next.m;
				next.m = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}
		
				if(a.s < sf.s && a.m)		//m给s倒 
		{
			pingzi next = a;
			int b =  sf.s - next.s;
			if(b <= a.m)		//当b小于a.s时
			{
				next.m -= b;
				next.s = sf.s; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.s += next.m;
				next.m = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}
		
		if(a.s < sf.s && a.n)		//n给s倒 
		{
			pingzi next = a;
			int b =  sf.s - next.s;
			if(b <= a.n)		//当b小于a.s时
			{
				next.n -= b;
				next.s = sf.s; 
				//printf("sf.m = %d\n",b); 
			}
			else				//当b大于a.s时 
			{
				next.s += next.n;
				next.n = 0; 
			}
			if(step[next.s][next.m][next.n] == 0)
			{
				step[next.s][next.m][next.n] = step[a.s][a.m][a.n] + 1;
				p.push(next);
			}
		}
	}
	return 0;
}
int main()
{
	while(1)
	{
		int s,n,m;
		scanf("%d%d%d",&s,&m,&n);
		if(s == 0 && n == 0 && m == 0)
		break;
		if(s % 2 == 1)
		{
			printf("NO\n");
			continue;
		}
		sf.m = m;
		sf.n = n;
		sf.s = s;
		int a = bfs();
		if(a > 0)
		printf("%d\n",a - 1);
		else
		printf("NO\n");
	}
	return 0;
}
