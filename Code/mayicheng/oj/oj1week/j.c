#include <stdio.h>
#include <string.h>
#include <math.h>
int p(int t)
{
	int sum = 0;
	int i;
	for(i = 1;i <= t / 2 + 1;i++)
	{
		if(t / i == ceil(1.0 * t / i * 1.0))
		{
			sum += i;
		}
	}
	return sum;
}
int main()
{
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int a,b;
		scanf("%d%d",&a,&b);
		if(p(a) == b && p(b) == a)
		{
			printf("YES\n");
		}
		else
		{
			printf("NO\n");
		}
	}
	return 0;
}
