/*************************************************************************
	> File Name: 201802D.c
	> Author:
	> Mail:
	> Created Time: 2018年07月31日 星期二 20时41分32秒
 ************************************************************************/

#include<stdio.h>
#include<math.h>
double two(int n)
{
  int num=1,num1=1;
  if(num%n==0)
  return num;
  while(1)
  {
    if((num*=10)%n==0)
    return num;
    //printf("%d    ",num);
    if((num+=1)%n==0)
    return num;
    if((num1*=10)%n==0)
    return num1;
    //printf("%d   %d\n",num,num1);
  }
}
int main()
{
  int n;

  while(scanf("%d",&n)!=EOF&&n!=0)
  {
    printf("%d\n",two(n));
  }
}
