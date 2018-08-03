#include <stdio.h>
#include <string.h>

int m, n;

#define isok(x, y) ((x) >= 0 && (x) < m && (y) >= 0 && (y) < n)

char p[100][100];
int vis[100][100];

void dfs(int x, int y)
{
    static int dx[] = {0, -1, 0, 1, 1, -1, -1, 1};
    static int dy[] = {-1, 0, 1, 0, 1, -1, 1, -1};
    int i;

    if (!isok(x, y) || vis[x][y] || p[x][y] != '@')
        return;
    vis[x][y] = 1; 
    for (i = 0; i < 8; i++)
        dfs(x + dx[i], y + dy[i]);
}

int main(void)
{
    while (scanf("%d%d", &m, &n) == 2 && m && n) {
        int i, j;
        int depth = 0;
       
        if (isok(2, 100))
            ;
        memset(p, 0, sizeof(p));
        memset(vis, 0, sizeof(vis));
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++)
                scanf(" %c", &p[i][j]);
        }

        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                if (!vis[i][j] && p[i][j] == '@') {
                    depth++;
                    dfs(i, j);
                }
        printf("%d\n", depth);
    }
    return 0;
}
