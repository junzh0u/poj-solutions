#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stack>

using namespace std;

const long MAXN = 2005;

stack <long> S;

bool connect[MAXN][MAXN];
bool reach[MAXN];
long love[MAXN][MAXN], path[MAXN][MAXN], unpath[MAXN][MAXN];
long link[MAXN], marry[MAXN], father[MAXN];
long queue[MAXN], hq;
long n, root;

void DFSA(long now)
{
	long k;
	reach[now] = true;
	for (k = 1; k <= path[now][0]; k ++)
		if (! reach[path[now][k]])
			DFSA(path[now][k]);
	S.push(now);
}

void DFSB(long now)
{
	long k;
	reach[now] = true;
	queue[hq ++] = now;
	for (k = 1; k <= unpath[now][0]; k ++)
		if (! reach[unpath[now][k]])
			DFSB(unpath[now][k]);
}

int main()
{
	long i, j, k, t;

	scanf("%ld", & n);

	for (k = 1; k <= n; k ++)
	{
		scanf("%ld", & love[k][0]);

		for (i = 1; i <= love[k][0]; i ++)
			scanf("%ld",& love[k][i]);
	}

	for (k = 1; k <= n; k ++)
	{
		scanf("%ld", & i);
		marry[k] = i;
		link[i] = k;
	}

	for (k = 1; k <= n; k ++)
	{
		t = love[k][0];
		std::sort(love[k] + 1, love[k] + t + 1);
	}

	for (i = 1; i <= n; i ++)
		for (j = 1; j <= love[i][0]; j ++)
		{
			k = love[i][j];
			k = link[k];
			path[i][0] ++;
			path[i][path[i][0]] = k;

			unpath[k][0] ++;
			unpath[k][unpath[k][0]] = i;
		}

	memset(reach, false, sizeof(reach));
	for (k = 1; k <= n; k ++)
		if (! reach[k])
			DFSA(k);

	memset(reach, false, sizeof(reach));
	hq = 0;
	for (k = 1; k <= n; k ++)
	{
		if (! reach[S.top()])
		{
			hq = 0;
			DFSB(S.top());
			for (i = 0; i < hq; i ++)
				for (j = 0; j < hq; j ++)
					connect[queue[i]][marry[queue[j]]] = true;
		}
		S.pop();
	}

	for (i = 1; i <= n; i ++)
	{
		k = 0;
		for (t = 1; t <= love[i][0]; t ++)
		{
			j = love[i][t];
			if (connect[i][j])
				k ++;
		}
		printf("%ld", k);
		for (t = 1; t <= love[i][0]; t ++)
		{
			j = love[i][t];
			if (connect[i][j])
				printf(" %ld", j);
		}
		printf("\n");
	}
	return 0;
}
