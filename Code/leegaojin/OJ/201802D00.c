#include<stdio.h>
 /*只有两种情况，bfs 一种是*10,  另一种是*10 + 1 */
 long long r(int num, int count, long long sum, int *flag, long long *sum1)
 {
 	if(count > 19)
 		return -1;
 	else
 	{
 		if(*flag == -1)
 			return *sum1;
 		else if(sum % num == 0)
 		{
 			*flag = -1;
 			*sum1 = sum;
 			return *sum1;
 		}
 		else
 		{
 			  r(num, count + 1, sum * 10, flag, sum1);
 			  r(num, count + 1, sum * 10 + 1, flag, sum1);
 		}
 	}
 }

 int main(void)
 {
 	int n;
 	int count = 1;
 	long long sum = 1;
 	long long sum1 = 0;
 	int flag = 0;
 	scanf("%d", &n);
 	while(n != 0)
 	{
 		sum1 = r(n, count, sum, &flag, &sum1);
 		if(sum1 == -1);
 		else
 			printf("%lld\n", sum1);
 		flag = 0;
 		sum1 = 1;
 		scanf("%d", &n);
 	}
 	return 0;
 }
