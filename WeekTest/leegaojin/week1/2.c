/*************************************************************************
	> File Name: 2.c
	> Author:
	> Mail:
	> Created Time: 2018年07月28日 星期六 16时22分31秒
 ************************************************************************/

#include<stdio.h>
int convert(const char*num)
{
  //const char*strc=num;
  int temp=0;
  while(*num!=NULL)
  {
    if(*num>'9'||*num<'0'){
      break;
    }
    temp =temp*10+(*num-'0');
    num++;
  }
  return temp;
}
int main()
{
  char str[100];
  int num;
  gets(str);
  num= convert(str);
  printf("%d\n",num);
  return 0;
}
