#include <stdio.h>
#include <string.h>
#include <math.h>
int main()
{
	int m;
	scanf("%d",&m);
	while(m--)
	{
		float a,b,c;
		scanf("%f%f%f",&a,&b,&c);
		if(a <= 0 || b <= 0 || c <= 0)
		{
			printf("NO\n");
			continue;
		}
		if(a + b > c && a + c > b && b + c > a)
		{
			//if(abs(a -b) < c && abs(a - c) < b && abs(b - c) < a)
		//	{
				printf("YES\n");
				continue;
		//	}
		}
		else
		printf("NO\n");
		
	}
	return 0;
}
