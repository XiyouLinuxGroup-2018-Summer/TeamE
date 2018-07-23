#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX 105
int main()
{
	int a[MAX] = { 0 };
	int n;
	int m;
	while(1)
	{
		int flag  = 1;
		scanf("%d%d",&n,&m);
		if(n == 0 && m == 0)
			break;
		int i = 0;
		for(i = 0;i <= n;i++)
		{
			scanf("%d",&a[i]);
			if(m < a[i] && flag)
			{
				a[i + 1] = a[i];
				a[i] = m;
				i++;
				flag = 0;
			}
		}
		for(i = 0;i <= n;i++)
		{
			printf("%d",a[i]);
			if(i < n)
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}
