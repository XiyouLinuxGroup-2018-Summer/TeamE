#include <stdio.h>
void qsort(char *a,int b,int c)
{
	if(b >=  c)
	return;
	char key = a[b];
	int i = b;
	int j = c;
	while(i < j)
	{
		while(i < j && a[j] >= key) j--;
		a[i] = a[j];
		while(i < j && a[i] <= key) i++;
		a[j] = a[i];
	}
	a[i] = key;
	qsort(a,b,i - 1);
	qsort(a,i + 1,c);
}
int main()
{
	char a[4] = {0};
	while(scanf("%s",a) != EOF)
	{
		qsort(a,0,2);
		printf("%c %c %c\n",a[0],a[1],a[2]);
	}
	return 0;
}
