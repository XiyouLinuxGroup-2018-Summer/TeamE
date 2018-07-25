#include <stdio.h>

int main(void)
{
    int n;
    float a, b, c;

    scanf("%d", &n);
    while (n-- > 0) {
        scanf("%f%f%f", &a, &b, &c);
        if (a > 0 && b > 0 && c > 0 && a + b > c && b + c > a && a + c > b)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
