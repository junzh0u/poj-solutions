#include <iostream.h>
#include <memory.h>
#include <algorithm>

const ax[4] = {-1, 0, 1, 0}, ay[4] = {0, 1, 0, -1};

struct Ta
{
	int x1, y1, x2, y2;
} a[105];
struct Tq
{
	short x, y;
} q[40020];
bool c[210][210][4], mark[210][210];
int x0[210], y0[210];
int s1, s2;

int get(int x, int ss, int xx[])
{
	int s = 0, t = ss - 1, mid;
	while (s <= t)
	{
		mid = (s + t) / 2;
		if (x == xx[mid])
			return mid;
		else
			if (x < xx[mid])
				t = mid - 1;
			else
				s = mid + 1;
	}
	return -1;
}

void search0(int nx, int ny)
{	
	int qs = -1, qt = 0, r;
	q[0].x = nx; q[0].y = ny;
	mark[nx][ny] = 1;
	while (qs < qt)
	{
		qs++;
		for (r = 0; r < 4; r++)
		{
			nx = q[qs].x + ax[r];
			ny = q[qs].y + ay[r];
			if (nx < 0 || ny < 0 || nx > s1 || ny > s2 || mark[nx][ny] || c[q[qs].x][q[qs].y][r])
				continue;
			mark[nx][ny] = 1;
			qt++;
			q[qt].x = nx; q[qt].y = ny;
		}
	}	
}

void search(int nx, int ny)
{	
	int qs = -1, qt = 0, r;
	q[0].x = nx; q[0].y = ny;
	mark[nx][ny] = 1;
	while (qs < qt)
	{
		qs++;
		for (r = 0; r < 4; r++)
		{
			nx = q[qs].x + ax[r];
			ny = q[qs].y + ay[r];
			if (nx < 0 || ny < 0 || nx > s1 || ny > s2 || mark[nx][ny])
				continue;
			mark[nx][ny] = 1;
			qt++;
			q[qt].x = nx; q[qt].y = ny;
		}
	}
}

int main()
{
	int i, j, x1, y1, x2, y2, n, ans, tt;

	cin >> n;
	while (n != 0)
	{
		s1 = s2 = 0;
		for (i = 0; i < n; i++)
		{
			cin >> a[i].x1 >> a[i].y1 >> a[i].x2 >> a[i].y2;
			x0[s1++] = a[i].x1;
			y0[s2++] = a[i].y1;
			if (a[i].x1 != a[i].x2)
				x0[s1++] = a[i].x2;
			else
				y0[s2++] = a[i].y2;
		}
		std::sort(x0, x0 + s1);
		std::sort(y0, y0 + s2);
		j = 0;
		for (i = 1; i < s1; i++)
			if (x0[i] != x0[j])
				x0[++j] = x0[i];
		s1 = j + 1;
		j = 0; 
		for (i = 1; i < s2; i++)
			if (y0[i] != y0[j])
				y0[++j] = y0[i];
		s2 = j + 1;
		memset(c, 0, sizeof(c));
		for (i = 0; i < n; i++)
		{
			x1 = get(a[i].x1, s1, x0);
			x2 = get(a[i].x2, s1, x0);
			y1 = get(a[i].y1, s2, y0);
			y2 = get(a[i].y2, s2, y0);
			if (x1 > x2)
			{
				tt = x1; x1 = x2; x2 = tt;
			}
			if (y1 > y2)
			{
				tt = y1; y1 = y2; y2 = tt;
			}
			if (x1 == x2)
				for (j = y1 + 1; j <= y2; j++)
					c[x1][j][2] = c[x1 + 1][j][0] = 1;
			else
				for (j = x1 + 1; j <= x2; j++)
					c[j][y1][1] = c[j][y1 + 1][3] = 1;
		}
		memset(mark, 0, sizeof(mark));

		search0(0, 0);
		ans = 0;
		for (i = 0; i <= s1; i++)
			for (j = 0; j <= s2; j++)
				if (!mark[i][j])
				{
					search(i, j);
					ans++;
				}
		cout << "There are " << ans << " holes.\n";
		cin >> n;
	}
	return 0;
}

