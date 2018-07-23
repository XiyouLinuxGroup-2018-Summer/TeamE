#include <stdio.h>
#include <string.h>
#include <math.h>
int main()
{
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int a,b;
		char c;
		c = getchar();
		if(c != '+' || c != '-' || c != '*' || c != '/')
		{
			c = getchar();
		}
		scanf("%d%d",&a,&b);
		if(c == '+')
		{
			printf("%d\n",a + b);
		}
		if(c == '-')
		{
			printf("%d\n",a - b);
		}
		if(c == '*')
		{
			printf("%d\n",a * b);
		}
		if(c == '/')
		{
			if(a / b == ceil(1.0 * a / b * 1.0))
			{
				printf("%d\n",a/b);
			}
			else
			{
				float mm = 1.0 * a / b * 1.0;
				printf("%.2f\n",mm);
			}
		}
	}
	return 0;
}
