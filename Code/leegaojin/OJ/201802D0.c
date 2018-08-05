#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int f;
void dfs(unsigned long long int  x,int k,int n)
{
        if(f)
            return ;
         if(x%n==0)
         {
             printf("%llu\n",x);
             f=1;
             return ;
         }
         if(k==19)
             return ;
         dfs(x*10,k+1,n);
         dfs(x*10+1,k+1,n);
}
int main()
{
    int n;
    while(scanf("%d",&n)&&n)
    {
        f=0;
        dfs(1,0,n);
    }
}
