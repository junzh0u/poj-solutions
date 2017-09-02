#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

const ax[4] = {0, 1, 0, -1}, ay[4] = {1, 0, -1, 0};

struct Tnode
{
	short x, y;
} p[30], w[30], q[30], d[30];						//分别可扩展点队列，为“*”的格子，队列

char g[101][101];									//读入的数据
short mark[101][101] = {0};							//标记某个格子是否为可扩展点
bool cover[101][101] = {0}, c[16][16], u[6][6];		//分别为A集合标记，B集合标记，连通性
int n, m, tt;

inline bool match(short minx2, short miny2, short maxx2, short maxy2, short maxx1, short maxy1)
{													//通过平移判断两个集合是否刚好构成N*N的图形
	short rx, ry, j;
	bool suc;

	for (rx = maxx2 - n + 1; rx < n + minx2 - maxx1; rx++)
		for (ry = maxy2 - n + 1; ry < n + miny2 - maxy1; ry++)
		{
			suc = 1;
			for (j = 0; j < tt; j++)
				if (c[d[j].x + rx][d[j].y + ry])
				{
					suc = 0;
					break;
				}
			if (suc)
				return true;
		}
	return false;
}

inline short connect(int sx, int sy)
{													//判断B集合中的连通性
	int qs = 0, qt = 1, r, nx, ny;
	q[qt].x = sx; q[qt].y = sy; u[sx][sy] = 0;
	while (qs < qt)
	{
		qs++;
		for (r = 0; r < 4; r++)
		{
			nx = q[qs].x + ax[r]; ny = q[qs].y + ay[r];
			if (nx < n && nx >= 0 && ny < n && ny >= 0 && u[nx][ny])
			{
				u[nx][ny] = 0;
				qt++;
				q[qt].x = nx; q[qt].y = ny;
			}
		}
	}
	return qt;
}

inline bool ok(short minx2, short miny2, short maxx2, short maxy2)
{			//返回true则成功，可以输出；否则失败。传入的参数为A集合边界
	short minx1, miny1, maxx1, maxy1, i, j, tmp;

	minx1 = miny1 = 200; maxx1 = maxy1 = 0;
	for (i = 0; i < m; i++)					//记录B集合的边界
		if (!cover[w[i].x][w[i].y])
		{
			if (w[i].x < minx1)
				minx1 = w[i].x;
			if (w[i].x > maxx1)
				maxx1 = w[i].x;
			if (w[i].y < miny1)
				miny1 = w[i].y;
			if (w[i].y > maxy1)
				maxy1 = w[i].y;
		}

	memset(c, 0, sizeof(c));
	tt = 0;
	for (i = 0; i < m; i++)
		if (!cover[w[i].x][w[i].y])
		{
			if (w[i].x - minx1 >= n || w[i].y - miny1 >= n)	//若超过N*N则失败
				return false;
			d[tt].x = w[i].x - minx1;						//将B集移动到左上角
			d[tt].y = w[i].y - miny1;
			tt++;
		}
		else
			c[w[i].x - minx2 + 5][w[i].y - miny2 + 5] = 1;	//将A集移动到右上角

	memset(u, 0, sizeof(u));	//为判断连通性初始化
	for (i = 0; i < tt; i++)
		u[d[i].x][d[i].y] = 1;
	if (connect(d[0].x, d[0].y) < tt)	//若不连通则失败
		return false;
	maxx2 = maxx2 - minx2 + 5; maxy2 = maxy2 - miny2 + 5;
	minx2 = miny2 = 5;
	maxx1 = maxx1 - minx1; maxy1 = maxy1 - miny1;
	minx1 = miny1 = 0;

	for (i = 0; i < 4; i++)				//4次旋转
	{
		if (match(minx2, miny2, maxx2, maxy2, maxx1, maxy1))
			return true;				//通过平移看能否构成N*N的图形
		minx1 = miny1 = 200; maxx1 = maxy1 = 0;
		for (j = 0; j < tt; j++)
		{								//旋转过程：坐标进行变换
			tmp = d[j].y; d[j].y = n - d[j].x - 1;
			d[j].x = tmp;
			if (d[j].x < minx1)
				minx1 = d[j].x;
			if (d[j].x > maxx1)
				maxx1 = d[j].x;
			if (d[j].y < miny1)
				miny1 = d[j].y;
			if (d[j].y > maxy1)
				maxy1 = d[j].y;
		}
		for (j = 0; j < tt; j++)		//将图形移动到左上角
		{
			d[j].x -= minx1; d[j].y -= miny1;
		}
		maxx1 -= minx1; maxy1 -= miny1;
	}
	return false;
}

bool search(short minx, short miny, short maxx, short maxy, short t, short q)
{
	short r, nx, ny;

	if (q > t)							//若当前已经没有可扩展点，则退出
		return false;

	if (search(minx, miny, maxx, maxy, t, q + 1))	//第q个点不属于A集合
		return true;					//若有解则返回成功标志

	//第q个点属于A集合

	if (abs(minx - p[q].x) >= n || abs(maxx - p[q].x) >= n ||
		abs(miny - p[q].y) >= n || abs(maxy - p[q].y) >= n)
		return false;					//若不能在N*N范围内，则失败，返回

	for (r = 0; r < 4; r++)				//找出所有的可扩展点
	{
		nx = p[q].x + ax[r]; ny = p[q].y + ay[r];
		if (g[nx][ny] == '*' && !mark[nx][ny])
		{
			t++;
			p[t].x = nx; p[t].y = ny;
			mark[nx][ny] = q;			//可扩展点进行标记
		}
	}

	nx = p[q].x; ny = p[q].y;
	cover[nx][ny] = 1;					//当前点属于A集合
	if (!ok(__min(minx, nx), __min(miny, ny), __max(maxx, nx), __max(maxy, ny)))
	{									//若不能成功构成N*N，则继续搜索
		if (search(__min(minx, nx), __min(miny, ny), __max(maxx, nx), __max(maxy, ny), t, q + 1))
			return true;
	}
	else
		return true;					//若成功达到目标，则返回
	cover[nx][ny] = 0;					//清除标记

	for (r = 0; r < 4; r++)
	{
		nx = p[q].x + ax[r]; ny = p[q].y + ay[r];
		if (mark[nx][ny] == q)
			mark[nx][ny] = 0;
	}	
	return false;
}

int main()
{
	int i, j, line;

	line = 0;
	while (gets(g[line++]));		//读入全部数据

	m = 0;
	for (i = 0; i < line; i++)
		for (j = 0; g[i][j] != 0; j++)
			if (g[i][j] == '*')		//若当前是“*”，则记录下来
			{
				w[m].x = i; w[m].y = j;
				m++;
			}
	n = (int) sqrt(m);				//求出N
	p[1].x = w[0].x; p[1].y = w[0].y; mark[w[0].x][w[0].y] = -1;
	search(w[0].x, w[0].y, w[0].x, w[0].y, 1, 1);		//开始搜索
	for (i = 0; i < line; i++)		//输出
	{
		for (j = 0; g[i][j] != 0; j++)
			if (g[i][j] == '*')
				if (cover[i][j])
					printf("A");
				else
					printf("B");
			else
				printf(".");
		printf("\n");
	}
	return 0;
}
