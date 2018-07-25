#include <stdio.h>

int main(void)
{
    int n, m;
    int a;
    int flag;

    while (scanf("%d%d", &n, &m) == 2 && (n != 0 || m != 0)) {
        if (n < 0 || n > 100)
            return 1;
        flag = 0;
        while (n-- > 0) {
            scanf("%d", &a);
            if (flag)
                printf("%d", a);
            else {
                if (m > a)
                    printf("%d", a);
                else {
                    printf("%d %d", m, a);
                    flag = 1;
                }
            }
            printf("%c", n > 0 ? ' ' : '\n');
        }
    }
    return 0;
}
