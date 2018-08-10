#include<stdio.h>
int main()
{
  printf("%d\n",&(&3[(int *)1])[2]);
}
