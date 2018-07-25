#include <stdio.h>

int gcd(int a, int b)
{
    int r;

    while (b > 0) {
        r = a % b;
        a = b; 
        b = r;
    }
  return a;
}

int main(void)
{
    int a, b;

    while (scanf("%d%d", &a, &b) == 2)
        printf("%d\n", a * b / gcd(a, b));
    return 0;
}
