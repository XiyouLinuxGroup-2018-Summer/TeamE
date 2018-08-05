#include <stdio.h>

int main(void){

	printf("%d\n", (int *)3);
	printf("%d\n",&4[(int *)3]);
	printf("%d\n",&(&4[(int *)3])[5]);
	return 0;
}
