#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX 1005
int main()
{
	char c[MAX] = { 0 };
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int a = 0;
		scanf("%s",c);
		int t = strlen(c);
		int i = 0;
		for(i = 0;i < t;i++)
		{
			if(c[i] >= 48 && c[i] < 58)
			{
				a++;
			}
		}
		printf("%d\n",a);
	}
	return 0;
}
