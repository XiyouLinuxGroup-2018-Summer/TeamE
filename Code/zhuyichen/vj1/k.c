#include <stdio.h>
#include <math.h>

int is_prime(int n)
{
    int i;

    for (i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int main(void)
{
    int x, y;
    int n;

    while (scanf("%d%d", &x, &y) == 2 && (x != 0 || y != 0)) {
        if (x >= y)
            return 1;
        for (n = x; n <= y; n++)
            if (!is_prime(n * n + n + 41))
                break;
        if (n > y)
            printf("OK\n");
        else
            printf("Sorry\n");
    }
    return 0;
}
