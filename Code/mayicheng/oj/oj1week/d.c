#include <stdio.h>

int main()
{
	int a,b;
	while(scanf("%d%d",&a,&b) != EOF)
	{
		int aaa = a * b;
		if(a == 0 || b == 0)
		{
			printf("0\n");
			continue;
		}
		int mm = a;
		while(a != b)
		{
			if(a < b)
			{
				int t = a;
				a = b;
				b = t;
			}
			while(a >= b)
			{
				a -= b;
			}
			if(a == 0)
			{
				mm = b;
				break;
			}
		}
		if(mm == 0)
		{
			printf("0\n");
			continue;
		}
		printf("%d\n",aaa / mm);
	}
	return 0;
}
