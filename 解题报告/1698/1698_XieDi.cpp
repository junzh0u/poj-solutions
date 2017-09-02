#include <iostream.h>
#include <memory.h>

const maxint = 1000000;

short g[380][380], d[380];					//邻接表
char c[380][380], f[380][380];				//邻接矩阵
int q[400], fa[400], s, t;					//队列和增广路径
bool v[400];								//前向弧标记

void addedge(int u, int v, int f0)			//从u到v连一条容量f0的弧
{
	d[u]++;
	g[u][d[u]] = v;
	d[v]++;
	g[v][d[v]] = u;
	c[u][v] = f0;
	f[u][v] = 0;
}

bool findpath()								//找增广路径
{
	int qs = 0, qt = 1, r, u;
	q[qt] = s;
	for (r = 0; r <= t; r++)
		fa[r] = maxint;
	fa[s] = s;
	while (qs < qt)
	{
		qs++;
		u = q[qs];
		for (r = 1; r <= d[u]; r++)
			if (fa[g[u][r]] == maxint)
				if (f[u][g[u][r]] < c[u][g[u][r]] || f[g[u][r]][u] > 0)
				{
					q[++qt] = g[u][r];
					if (f[u][g[u][r]] < c[u][g[u][r]])
					{
						v[g[u][r]] = 1;
						fa[g[u][r]] = u;
					}
					else
					{
						v[g[u][r]] = 0;
						fa[g[u][r]] = u;
					}
				}
	}
	return fa[t] != maxint;
}

void addpath()								//改进增广路
{
	int i;

	i = t;
	while (i != s)
	{
		if (v[i])
			f[fa[i]][i]++;
		else
			f[i][fa[i]]--;
		i = fa[i];
	}
}

int maxflow()								//求最大流
{
	int flow = 0;
	while (findpath())
	{
		addpath();
		flow++;
	}
	return flow;
}

int main()
{
	int q, i, j, k, mw, w, tot, day, n, now;
	short a[7];
	cin >> q;
	for (; q > 0; q--)
	{
		memset(d, 0, sizeof(d));
		memset(c, 0, sizeof(c));
		memset(f, 0, sizeof(f));		
		cin >> n; mw = 0; s = 0; tot = 0;	//以下是构图
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < 7; j++)
				cin >> a[j];
			cin >> day >> w;
			tot += day;
			if (w > mw)
				mw = w;
			addedge(s, i + 1, day);
			for (j = 0; j < w; j++)
				for (k = 0; k < 7; k++)
					if (a[k])
						addedge(i + 1, n + j * 7 + k + 1, 1);
		}
		t = n + mw * 7 + 1;
		for (j = 0; j < mw; j++)
			for (k = 0; k < 7; k++)
				addedge(n + j * 7 + k + 1, t, 1);

		now = maxflow();
		if (now == tot)						//如果全部电影都能够拍摄，则输出“Yes”
			cout << "Yes\n";
		else								//否则输出“No”
			cout << "No\n";
	}
	return 0;
}
