#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stack>
#define ll long long
//top()、push()、pop()、 size()、empty()
using namespace std;
stack <long long>p;
double mm = 0;
long long xiangcheng(long long t,const int a)
{
	while(!p.empty())
	p.pop();
	p.push(t);
	while(!p.empty())
	{
		ll now = p.top();
		//printf("%lld\n",now);
		p.pop();
		//printf("%I64u\n",now);
		if((now % a) == 0)
		{	
		//	printf("%I64u\n",now);
		//	printf("%d",ceil(1.0 * now / a * 1.0));
		//	printf("t / a = %lf\n",1.0 * now / a * 1.0);
			return now;
		}
		if((now * 10 + 1) < 10000000000000000000)
		p.push(now * 10 + 1);
		if((now * 10) < 10000000000000000000)
		p.push(now * 10);

	}
}
int main()
{
	float p = 1;
	while(1)
	{
		p *= 0.1;
		printf("%lf\n",p);
	}
//	printf("%lf\n",1.0 * 1111 / 99 );
////	if(111111111111111111LL % 99 == 0)
//	printf("%lld",111111111111111111LL/99);
//	printf("%llf",10.0/20);
	//11111111111111112.000000
	int a;
	while(scanf("%d",&a) != EOF)
	{
		if(a == 0)
		break;
		mm = 0;
		long long t = 1;
		cout << xiangcheng(t,a) << endl;
	}
	return 0;
}
