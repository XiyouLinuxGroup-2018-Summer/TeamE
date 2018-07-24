#include<stdio.h>

int main(void)
{
	int x, y, i, a, b;
	while(scanf("%d%d", &x, &y)!=EOF){
		a = x, b = y;
		for (i = 1; i <= x; i++)
		{
			if (x%i == 0 && y%i == 0)
				break;
		}
		for (i = x; i > 0; i--)
		{
			if (x%i == 0 && y%i == 0)
				break;
		}
	
		printf("%d\n", a*b / i);
	}
	return 0;
}
