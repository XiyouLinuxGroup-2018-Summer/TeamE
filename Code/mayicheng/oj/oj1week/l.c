#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 105
int main()
{
	char c[MAX] = { 0 };
	while(fgets(c,sizeof(c),stdin) != NULL)
	{
		int t = strlen(c);
		int i = 0;
		c[0] -= 32;
		for(i = 0;i < t;i++)
		{
			if(c[i] == ' ' && c[i + 1] != ' ')
			{
				c[i + 1] -= 32;
			}
		}
		printf("%s",c);
	}
	return 0;
}
