#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *p = fopen("a.txt","r");
	while(1)
	{
		char c[100];
		int m = fread(c,10,1,p);
		printf("m = %d    %s\n",m,c);
		if(m == 0)
		break;
	}
	return 0;
}
