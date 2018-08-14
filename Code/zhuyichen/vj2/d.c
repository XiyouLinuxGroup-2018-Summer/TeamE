#include <stdio.h>

int flag;

void dfs(int n, int depth, size_t b)
{
    if (flag)
        return;
    if (b % n == 0) {
        printf("%lu\n", b);
        flag = 1;
        return;
    }
    if (depth >= 19)
        return;

    dfs(n, depth + 1, b * 10);
    dfs(n, depth + 1, b * 10 + 1);
}

int main(void)
{
    int n;

    while (scanf("%d", &n) == 1 && n != 0) {
        flag = 0;
        dfs(n, 0, 1);
    }
    return 0;
}
