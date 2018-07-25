#include <stdio.h>

int common_divisor(int n)
{
    int i, sum;

    sum = 0;
    for (i = 1; i < n; i++)
        if (n % i == 0)
            sum += i;
    return sum;
}

int main(void)
{
    int n;
    int a, b;

    scanf("%d", &n);
    while (n-- > 0) {
        scanf("%d%d", &a, &b);
        if (a < 0 || a > 600000 || b < 0 || b > 600000)
            return 1;
        if (common_divisor(a) == b && common_divisor(b) == a)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
