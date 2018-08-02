#include <stdio.h>
#include <string.h>

struct que {
    int x;
    int dis;
} q[100001];

int main(void)
{
    int a, b;
    int i, tx;
    int front, rear;
    int vis[100001];

    while (scanf("%d%d", &a, &b) == 2) {
        memset(q, 0, sizeof(q));
        memset(vis, 0, sizeof(vis));

        front = rear = 0;
        q[rear].x = a;
        q[rear++].dis = 0;
        vis[a] = 1;

        while (front < rear) {
            if (q[front].x == b) {
                printf("%d\n", q[front].dis);
                break;
            }
            for (i = 0; i < 3; i++) {
                if (i == 0)
                    tx = q[front].x - 1;
                else if (i == 1)
                    tx = q[front].x + 1;
                else
                    tx = q[front].x * 2;
                if (tx >= 0 && tx <= 100000 && !vis[tx]) {
                    q[rear].x = tx;
                    q[rear++].dis = q[front].dis + 1;
                    vis[tx] = 1;                    
                }
            }
            front++;
        }
    }
    return 0; 
}
