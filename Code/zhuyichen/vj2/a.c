#include <stdio.h>

struct que {
     int x;
     int y;
     int pre;   /*  记录上一个点  */
} q[5 * 5];   /*  模拟队列  */

/*  backpath函数：回溯路径  */
void backpath(int front)
{
    if (q[front].pre == -1) {
        printf("(%d, %d)\n", q[front].x, q[front].y);
        return;
    }
    backpath(q[front].pre);
    printf("(%d, %d)\n", q[front].x, q[front].y);
}

int main(void)
{
    int maze[5][5]; 
    int vis[5][5] = { {0} };   /*  标记是否访问过  */
    int dx[] = {1, 0, 0, -1};
    int dy[] = {0, 1, -1, 0};
    int front, rear;
    int tx, ty;
    int i, j;

    front = rear = 0;
    q[rear].x = 0; 
    q[rear].y = 0;
    q[rear++].pre = -1;
    vis[0][0] = 1;

    for(i = 0; i < 5; i++)
         for(j = 0; j < 5; j++)
             scanf("%d", &maze[i][j]);

    while (front < rear) {
        if (q[front].x == 4 && q[front].y == 4) {
            backpath(front);
            break;
        }
        for (i = 0; i < 4; i++) {   /*  搜索每一层  */
            tx = q[front].x + dx[i];
            ty = q[front].y + dy[i];
            if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5
                && !vis[tx][ty] && !maze[tx][ty]) {
                q[rear].x = tx;
                q[rear].y = ty;
                q[rear++].pre = front;
                vis[tx][ty] = 1;
            }
        }
        front++;
    }
    return 0;
}
