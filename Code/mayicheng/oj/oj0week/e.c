#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main()
{
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int m;
		scanf("%d",&m);
		int sum = 0;
		while(m--)
		{
			int a;
			scanf("%d",&a);
			sum += a;
		}
		printf("%d\n",sum);
	}
	return 0;
}
