#include <stdio.h>

int main(void)
{
    int n, m, a, sum;

    scanf("%d", &n);
    while (n-- > 0 && scanf("%d", &m) == 1) {
        sum = 0;
        while (m-- > 0) {
            scanf("%d", &a);
            sum += a;
        }
        printf("%d\n%s", sum, n ? "\n" : "");
    }
    return 0;
}
