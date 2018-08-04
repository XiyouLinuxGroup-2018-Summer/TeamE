#include "iostream"
#include "cmath"
#include "cstdio"
#include "cstdlib"
#include "cstring"
using namespace std;
char c[8][8] = { 0 };
char S[9] = { 0 };
int n,k;	//n是行和列，k是多少棋子
int sum;
int mm;//已放棋子的数目
void dfs(int a)	//a是行,mm是放了多少个棋子了 a是从0开始的 
{
	if(mm == k)	//棋子够了也可以停了 
	{
		sum++;
		return;
	}
	if(a >= n)	//超了就越界了 
	return;
	int i = 0;
	for(i = 0;i < n;i++)
	{
		if(S[i] || (c[a][i] == 1))
		continue;
		S[i] = 1;
		mm++;
		dfs(a + 1);
		mm--;
		S[i] = 0;
	}
	dfs(a + 1);
}
int main()
{
	while(1)
	{
		memset(c,0,sizeof(c));
		memset(S,0,sizeof(S));
		sum = 0;
		scanf("%d%d",&n,&k);
		if(n == -1 && k == -1)
		break;
		getchar();
		int i,j;
		for(i = 0;i < n;i++)		//把'#'和'.'改成0和1，方便 
			for(j = 0;j < n;j++)
			{
				char a = getchar();
				while(a == ' ' || a == '\n')
					a = getchar();
				if(a == '#')
				c[i][j] = 0;
				if(a == '.')
				c[i][j] = 1;
			}
		dfs(0);
		printf("%d\n",sum);	
	}
}
