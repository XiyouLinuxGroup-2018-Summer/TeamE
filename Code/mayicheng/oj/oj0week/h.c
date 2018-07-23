#include <stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int m;
		int t = 0;
		scanf("%d",&m);
		while(m--)
		{
			int a;
			scanf("%d",&a);
			t += a;
		}
		printf("%d\n",t);
		if(n != 0)
		{
			printf("\n");
		}
	}
	return 0;
}
