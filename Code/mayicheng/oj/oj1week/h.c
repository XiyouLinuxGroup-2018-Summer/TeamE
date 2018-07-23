#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 105
int main()
{
	int n;
	char c[MAX] = { 0 };
	scanf("%d",&n);
	getchar();
	while(n--)
	{
		gets(c);
	//	getchar();
	//	printf("%s\n",c);
		if(c[0] >= 48 && c[0] < 58)
		{
			printf("no\n");
			continue;
		}
		if(c[0] == '_' || (c[0] >=65 && c[0] <= 90)||(c[0] >=97 && c[0] <=122))
		{
			
		}
		else
		{
			printf("no\n");
			continue;
		}
		int i = 0;
		int flag = 1;
		int t = strlen(c);
		for(i = 1;i < t;i++)
		{
			if(c[i] == '_'||(c[i] >= 65 && c[i] <= 90)||(c[i] >= 97 && c[i] <= 122)||(c[i]>= 48 && c[i] < 58))
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
			printf("yes\n");
		}
		else
		{
			printf("no\n");
		}
	//	printf("%s\n",c);
	}
	return 0;
}
