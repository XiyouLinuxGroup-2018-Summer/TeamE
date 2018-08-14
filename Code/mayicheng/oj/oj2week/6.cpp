#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
typedef struct gro
{
	int a;
}gro;
gro a[100][100] = { 0 };
int ssum = 0;
int M,N;
int dfs(int i,int j)
{
	if(i < 0 || i >= M || j < 0 || j >= N || a[i][j].a == 1)
	return 0;
	a[i][j].a = 1;
	dfs(i + 1,j);
	dfs(i - 1,j);
	dfs(i,j + 1);
	dfs(i,j - 1);
	dfs(i + 1,j + 1);
	dfs(i + 1,j - 1);
	dfs(i - 1,j + 1);
	dfs(i - 1,j - 1);
	return 0;
}
void tbfs()
{
	int i,j;
	for(i = 0;i < M;i++)
		for(j = 0;j < N;j++)
		{
			if(a[i][j].a == 0)
			{
				//printf("i = %d,j = %d  a[i][j] = %d\n",i,j,a[i][j].a);
				ssum++;
				dfs(i,j);
			}
		}
}
int main()
{
	while(1)
	{
		memset(a,0,sizeof(a));
		ssum = 0;
		int m,n;
		scanf("%d%d",&m,&n);
		if(m == 0 && n == 0)
		return 0;
		M = m;N = n;
		int i = 0;
		int j = 0;
		for(i = 0;i < M;i++)
			for(j = 0;j < N;j++)
			{
				char c = 0;
				scanf("%c",&c);
				while(c == ' ' || c == '\n')
				scanf("%c",&c);
				if(c == '@')
				a[i][j].a = 0;
				if(c == '*')
				a[i][j].a = 1;
			}
		tbfs();
		printf("%d\n",ssum);
	}
	return 0;
} 
