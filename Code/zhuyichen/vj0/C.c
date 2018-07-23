#include <stdio.h>

int main(void)
{
    int a, b;
    
    while (scanf("%d%d", &a, &b) == 2 && (a != 0 || b != 0))
        printf("%d\n", a + b);
    return 0;
}
