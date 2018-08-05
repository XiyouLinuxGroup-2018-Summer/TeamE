#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
typedef struct gro
{
	int a;
	int n[25];
}gro;
gro a[5][5] = { 0 };
int ff[25] = { 0 };
int mm = 0;
int l = 0;
int bfs(int i,int j)
{
	if(i == 4 && j == 4)
	{
		ff[l++] = 10 * (i + 1) + j;
		return 1;
	}
	if(i < 0 || i > 4 || j < 0 || j > 4 || a[i][j].a == 1)
	return 0;
	int abb = 0;
	for(;abb < 25;abb++)
	if(a[i][j].n[abb] == 0)
	break;
	a[i][j].n[abb] = 10 * (i + 1) + j;
	a[i][j].a = 1;
	if(a[i + 1][j].a == 0 && (i + 1) < 5)
	{
		memcpy(a[i + 1][j].n,a[i][j].n,sizeof(a[i][j].n));
		ff[mm++] = 10 * (i + 2) + j;
	}
	if(a[i - 1][j].a == 0 && (i - 1) >= 0)
	{
		memcpy(a[i - 1][j].n,a[i][j].n,sizeof(a[i][j].n));
		ff[mm++] = 10 * (i) + j;
	}
	if(a[i][j + 1].a == 0 && (j + 1) < 5)
	{
		memcpy(a[i][j + 1].n,a[i][j].n,sizeof(a[i][j].n));
		ff[mm++] = 10 * (i + 1) + j + 1;
	}
	if(a[i][j - 1].a == 0 && (j - 1) >= 0)
	{
		memcpy(a[i][j - 1].n,a[i][j].n,sizeof(a[i][j].n));
		ff[mm++] = 10 * (i + 1) + j - 1;
	}
	int t = ff[0];
	int k = 0;
	for(;k < mm - 1;k++)
	ff[k] = ff[k + 1];
	mm--;
	int abc = bfs(t / 10 - 1,t % 10);
	if(abc == 1)
	{
		ff[l++] = 10 * (i + 1) + j;
		return 1;
	}
	return 0;
}
int main()
{
	int i = 0;
	int j = 0;
	for(i = 0;i < 5;i++)
		for(j = 0;j < 5;j++)
			scanf("%d",&(a[i][j].a));
	bfs(0,0);
		int abb = 0;
	for(;abb < 25;abb++)
	{
		if(a[4][4].n[abb] == 0)
		break;
	}
	for(i = 0;i < abb;i++)
	{
		printf("(%d, %d)\n",a[4][4].n[i] / 10 - 1,a[4][4].n[i] % 10);
	}
	printf("(4, 4)\n");
	return 0;
} 
