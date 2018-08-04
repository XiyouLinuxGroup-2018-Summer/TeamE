#include <stdio.h>
#include <string.h>

#define min(x, y) ((x) < (y) ? (x) : (y))

struct node
{
    int v[3];
    int step;
} q[101 * 101 * 101];

int v[3];
int vis[101][101];    /*  只标记前两个杯子的状态  */

void bfs()
{
    int front = 0, tail = 0;
    int flag = 0;
    int tx[3];
    int i, j, k, res; 

    q[tail].v[0] = v[0];    /*  初始状态，s = s, m = n = 0  */
    q[tail].v[1] = 0;
    q[tail].v[2] = 0;
    q[tail++].step = 0;
    vis[v[0]][v[1]] = 1;

    while(front < tail) {
        if(q[front].v[0] == v[0] / 2 && q[front].v[2] == 0) {
            printf("%d\n", q[front].step);
            flag = 1;
            break;
        }
        for(i = 0; i < 3; i++) {    /*  就是v[i]给v[j]倒  */
            for(j = 0; j < 3; j++) {
                if(i == j) 
                    continue;   /*  不能自己给自己倒  */
                res = min(q[front].v[i], v[j] - q[front].v[j]);
                for(k = 0; k < 3; k++) 
                    tx[k] = q[front].v[k];
                tx[i] = tx[i] - res;
                tx[j] = tx[j] + res;
                if (!vis[tx[0]][tx[1]]) {
                    vis[tx[0]][tx[1]] = 1;
                    q[tail].v[0] = tx[0];
                    q[tail].v[1] = tx[1];
                    q[tail].v[2] = tx[2];
                    q[tail++].step = q[front].step + 1;
                }
            }
        }
        front++;
    }
    if(flag == 0) 
        printf("NO\n");
}

int main(void)
{
    while(scanf("%d%d%d",&v[0],&v[1],&v[2]) == 3 && (v[0] != 0 || v[1] != 0 || v[2] != 0)) {
        if(v[0] % 2) {
            printf("NO\n");
            continue;
        }
        memset(vis, 0, sizeof(vis));
        if(v[1] < v[2]) {   /*  让v[1]成为第二大杯的饮料  */
            int tmp = v[1];
            v[1] = v[2];
            v[2] = tmp;
        }
        bfs();
    }
    return 0;
}
