#include <stdio.h>

int main(void)
{
    int n;
    char c;
    int a, b;

    scanf("%d", &n);
    getchar();

    if (n <= 0 || n >= 1000)
        return 1;
    
    while (n-- > 0) {
        scanf("%c%d%d", &c, &a, &b);
        getchar();
        if (a <= 0 || a >= 10000 || b <= 0 || b >= 10000)
            return 1;
        switch (c) {
        case '+': printf("%d\n", a + b); break;
        case '-': printf("%d\n", a - b); break;
        case '*': printf("%d\n", a * b); break;
        case '/':
            if (1.0 * a / b - (int)(1.0 * a / b) == 0)
                printf("%d\n", a / b);
            else
                printf("%.2f\n", 1.0 * a / b);
        default: break;
        }
    }
    return 0;
}
