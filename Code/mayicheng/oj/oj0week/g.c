#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main()
{
	int n;
	while(scanf("%d",&n) != EOF)
	{
		int sum = 0;
		while(n--)
		{
			int a;
			scanf("%d",&a);
			sum += a;
		}
		printf("%d\n",sum);
	}
	return 0;
}
