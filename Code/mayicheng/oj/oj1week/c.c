#include <stdio.h>
#include <string.h>
#define MAX 1005
int main()
{
	char a[MAX] = { 0 };
	int n;
	scanf("%d",&n);
	while(n--)
	{
		int i,j;
		int flag = 1;
		scanf("%s",a);
		int t = strlen(a);
		for(i = t - 1,j = 0;i  >= t / 2;i--,j++)
		{
			if(a[j] != a[i])
			{
				flag = 0;
			}
		}
		if(flag)
		{
			printf("yes\n");
		}
		else
		{
			printf("no\n");
		}
	}
	return 0;
}
