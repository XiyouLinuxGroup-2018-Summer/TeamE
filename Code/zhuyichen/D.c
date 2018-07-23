#include <stdio.h>

int main(void)
{
    int n, a, sum;

    while (scanf("%d", &n) == 1 && n > 0) {
        sum = 0;
        while (n-- > 0) {
            scanf("%d", &a);
            sum += a;
        }
        printf("%d\n", sum);
    }
    return 0;
}
