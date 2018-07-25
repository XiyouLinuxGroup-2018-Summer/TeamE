#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    int n;
    int c, count;

    scanf("%d", &n);
    getchar();
    while (n-- > 0) {
        count = 0;
        while ((c = getchar()) != '\n')
            if (isdigit(c))
                count++;
        printf("%d\n", count);
    }
    return 0;
}
