#include <stdio.h>
#include <math.h>
#include <memory.h>

const ax[4] = {-1, 0, 1, 0}, ay[4] = {0, 1, 0, -1};

bool b[22][22][4];
char fa[22][22];
int a[22][22];
struct Tq
{
	short x, y;
} q[500];
int n, m, r0;

int get_direct(int x1, int y1, int x2, int y2)		//判断(x1,y1), (x2,y2)构成的边的方向
{
	if (x1 == x2)
		if (y2 - y1 > 0)
			return 1;
		else
			return 3;
	else
		if (x2 - x1 > 0)
			return 2;
		else
			return 0;
}

void search(int sx, int sy)							//广度优先搜索
{
	int qs = -1, qt = 0, r, nx, ny;
	q[0].x = sx; q[0].y = sy;						//初始化
	fa[sx][sy] = -2;
	while (qs < qt)
	{
		qs++;
		for (r = 0; r < 4; r++)						//考察相邻格点
		{
			nx = q[qs].x + ax[r];
			ny = q[qs].y + ay[r];
			if (nx < 1 || nx > n || ny < 1 || ny > m || fa[nx][ny] != -1 || !b[q[qs].x][q[qs].y][r] || a[nx][ny] - a[q[qs].x][q[qs].y] > 10)
				continue;
			fa[nx][ny] = r;
			qt++;
			q[qt].x = nx; q[qt].y = ny;
		}
	}
}

void show(int x1, int y1, int x2, int y2)			//递归输出方案
{
	if (x1 != x2 || y1 != y2)
	{
		r0 = fa[x2][y2];
		show(x1, y1, x2 - ax[r0], y2 - ay[r0]);
		printf(" to ");
	}
	printf("%d-%d", x2, y2);
}

int main()
{
	int i, j, x1, y1, x2, y2, r;

	while (scanf("%d%d", &n, &m) == 2)
	{
		for (i = 1; i <= n; i++)					//输入数据
			for (j = 1; j <= m; j++)
				scanf("%d", &a[i][j]);
		memset(b, 0, sizeof(b));
		scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
		while (x1 != 0 || y1 != 0 || x2 != 0 || y2 != 0)
		{
			r = get_direct(x1, y1, x2, y2);			//将(x1, y1) 与 (x2, y2) 之间的点都标记是否可以到达
			while (x1 != x2 || y1 != y2)
			{
				b[x1][y1][r] = 1;
				x1 += ax[r];
				y1 += ay[r];
			}
			scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
		}
		scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
		while (x1 != 0 || y1 != 0 || x2 != 0 || y2 != 0)
		{
			if (x1 == x2 && y1 == y2)
				printf("To get from %d-%d to %d-%d, stay put!", x1, y1, x2, y2);
			else
			{
				memset(fa, -1, sizeof(fa));
				search(x1, y1);						//用广度优先搜索进行求解

				if (fa[x2][y2] == -1)
					printf("There is no acceptable route from %d-%d to %d-%d.", x1, y1, x2, y2);
				else
					show(x1, y1, x2, y2);
			}
			printf("\n\n");
			scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
		}
	}
	return 0;
}

