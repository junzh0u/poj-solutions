#include <iostream.h>
#include <string.h>

long l[5], c[5], u[5];
long dx, dy, res = 0, n, tot;

long Abs(long a)
{
	return a < 0 ? -a : a;
}

void Init()
{
	long xa, ya, xb, yb;
	long k;
	cin >> xa >> ya >> xb >> yb >> n;
	for (k = 0; k < n; k ++)				
		cin >> l[k];
	for (k = 0; k < n; k ++)
		cin >> c[k];
	dx = xa - xb;
	dy = ya - yb;
}

void Good(long loc, long dy, long already)
{
	long t;
	if (already > res)
		return;
	if (loc == n - 1)
	{
		if (dy % l[loc] != 0)
			return ;
		t = Abs(dy / l[loc]);
		if (t > c[loc] - u[loc])
			return;
		if (res > already + t)
			res = already + t;
		return;
	}
	long k;
	t = c[loc] - u[loc];
	for (k = -t; k <= t; k ++)
	{
		Good(loc + 1, dy - k * l[loc], already + Abs(k));
	}
}

void Search(long loc, long dx, long already)
{
	if (already > res)
		return;
	if (loc == n - 1)
	{
		if (dx % l[loc] != 0)
			return ;
		u[loc] = Abs(dx / l[loc]);
		if (u[loc] > c[loc])
			return;
		Good(0, dy, already + u[loc]);
		return;
	}
	long k;
	for (k = -c[loc]; k <= c[loc]; k ++)
	{
		u[loc] = Abs(k);
		Search(loc + 1, dx - k * l[loc], already + u[loc]);
	}
}

int main()
{
	Init();
	if (dx != 0 || dy != 0)
	{
		res = 100;
		Search(0, dx, 0);
		if (res == 100)
			res = -1;
	}
	cout << res << endl;
	return 0;
}

