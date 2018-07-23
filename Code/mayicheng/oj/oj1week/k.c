#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int pan(int t)
{
	if(t % 6 == 1 || t % 6 == 5)
	{
		int i = 2;
		for(;i <= sqrt(t) + 1;i++)
		{
			if(t / i == ceil(1.0 * t / i * 1.0))
			{
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}
}
int main()
{
	int x,y;
	while(1)
	{
		scanf("%d%d",&x,&y);
//		if(pan(x))
//		{
//			printf("ok\n");
//		}
//		else
//		{
//			printf("...\n");
//		}
		if(x == 0 && y == 0)
			break;
		int i;
		int flag = 1;
		for(i = x;i <= y;i++)
		{
			int t = i * i + i + 41;
			if(pan(t))
			{
				continue;
			}
			else
			{
				flag = 0;
				break;
			}
		}
		if(flag)
		{
			printf("OK\n");
		}
		else
		{
			printf("Sorry\n");
		}
	}
	return 0;
}
