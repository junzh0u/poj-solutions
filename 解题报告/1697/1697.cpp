#include <iostream.h>
#include <string.h>

const MAXN = 100;
const MAXV = 100000000;
int danger[MAXN][MAXN], map[MAXN][MAXN], rc[MAXN * MAXN * 2], dis[MAXN*MAXN]
	, pos[MAXN*MAXN];
int m, n, s0, t0, s, t;

void calcdanger()
{
	int i, j, x, y;

	t = 0;
	memset(danger, 0, sizeof(danger));
	for (i = 0; i <= n; i++)
		for (j = 0; j <= m; j++)
		if (map[i][j] || map[i+1][j] || map[i][j+1] || map[i+1][j+1])
		{
			danger[i][j] = 1;
			rc[++t] = i * (m+1) + j;
		}
	s = 1;
	while (s <= t)
	{
		x = rc[s] / (m+1); y = rc[s] % (m+1);
		if (x > 0 && !danger[x-1][y])
		{
			danger[x-1][y] = danger[x][y]+1;
			rc[++t] = rc[s]-m-1;
		}
		if (y > 0 && !danger[x][y-1])
		{
			danger[x][y-1] = danger[x][y]+1;
			rc[++t] = rc[s]-1;
		}
		if (x < n && !danger[x+1][y])
		{
			danger[x+1][y] = danger[x][y]+1;
			rc[++t] = rc[s]+m+1;
		}
		if (y < m && !danger[x][y+1])
		{
			danger[x][y+1] = danger[x][y]+1;
			rc[++t] = rc[s]+1;
		}
		s++;
	}
	for (i = 0; i <= n; i++)
		for (j = 0; j <= n; j++) 
			danger[i][j] = m+n+1-danger[i][j];
}

int getHead()
{
	int a, i, j, x;

	a = rc[1];
	x = rc[t--];
	i = 1;
	while (i * 2 <= t)
	{
		j = i * 2;
		if (j + 1 <= t && dis[rc[j]] > dis[rc[j+1]]) j = j + 1;
		if (dis[rc[j]] < dis[x])
		{
			rc[i] = rc[j]; pos[rc[i]] = i;
			i = j;
		}
		else break;
	}
	rc[i] = x; pos[x] = i;
	return a;

}

void add(int node)
{
	int x;
	if (pos[node] == 0)
	{
		rc[++t] = node; pos[node] = t;
	}
	x = pos[node];
	while (x > 1)
	{
		if (dis[rc[x/2]] > dis[node])
		{
			rc[x] = rc[x/2]; pos[rc[x]] = x;
			x = x / 2;
		}
		else break;
	}
	rc[x] = node; pos[node] = x;
}

void calcdis()
{
	int i, a, x, y;

	memset(pos, 0, sizeof(pos));
	for (i = 0; i <= (m+1)*(n+1); i++) dis[i] = MAXV;
	dis[s0] = danger[s0/(m+1)][s0%(m+1)];
	rc[t = 1] = s0; pos[s0] = 1;
	while (t > 0)
	{
		a = getHead();
		if (a == t0) break;
		x = a/(m+1); y = a%(m+1);
		if (x > 0 && (!map[x][y] || !map[x][y+1])
			 && dis[a-m-1] > danger[x-1][y]+dis[a])
		{
			dis[a-m-1] = danger[x-1][y]+dis[a];
			add(a-m-1);
		}
		if (y > 0 && (!map[x][y] || !map[x+1][y])
			 && dis[a-1] > danger[x][y-1]+dis[a])
		{
			dis[a-1] = danger[x][y-1]+dis[a];
			add(a-1);
		}
		if (x < n && (!map[x+1][y] || !map[x+1][y+1])
			 && dis[a+m+1] > danger[x+1][y]+dis[a])
		{
			dis[a+m+1] = danger[x+1][y]+dis[a];
			add(a+m+1);
		}
		if (y < m && (!map[x][y+1] || !map[x+1][y+1])
			 && dis[a+1] > danger[x][y+1]+dis[a])
		{
			dis[a+1] = danger[x][y+1]+dis[a];
			add(a+1);
		}
	}
	if (dis[t0] < MAXV) cout << dis[t0] << endl;
		else cout << "no solution" << endl;
}

void main()
{
	int i, j, test, c, r;
	char ch;

	cin >> test;
	while (test-- > 0)
	{
		cin >> n >> m;
		cin >> c >> r;
		s0 = c * (m + 1) + r;
		cin >> c >> r;
		t0 = c * (m + 1) + r;

		memset(map, 0, sizeof(map));
		for (i = 1; i <= n; i++)
			for (j = 1; j <= m; j++)
			{
				cin >> ch;
				map[i][j] = ch - '0';
			}

		calcdanger();
		calcdis();		
	}
}