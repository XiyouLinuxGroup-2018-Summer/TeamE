#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	char s[100] = {"32748"};
	int i = 0;
	int sum = 0;
	for(;i < strlen(s);i++)
	{
		sum += (s[i] - 48) * pow(10,(strlen(s) - i - 1));
	}
	printf("%d\n",sum);
} 
