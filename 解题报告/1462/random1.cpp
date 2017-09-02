#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#define zero 1e-8
#define MAXN 150
using namespace std;

struct Equation
{
  int a;
  double b;
};

double t[MAXN];
char p[MAXN][MAXN][MAXN], name[MAXN][MAXN];
int m[MAXN], N;
bool repeat;

void swap1(struct Equation x[MAXN], struct Equation y[MAXN])
{
  int i;
  for (i = 0; i <= MAXN - 1; i++)
  {
	swap(x[i].a, y[i].a);
	swap(x[i].b, y[i].b);
  }
}

void calc(int x)
{
  struct Equation e[MAXN][MAXN], now[MAXN], d[MAXN][MAXN];
  char *ss;
  double th1, th2, s;
  int i, j, k, n;
  
  for (i = 0; i <= MAXN - 1; i++)
  {
	for (j = 0; j <= MAXN - 1; j++)
    {
	  e[i][j].a = 0;
	  e[i][j].b = 0;
	  d[i][j].a = 0;
	  d[i][j].b = 0;
	}
    now[i].a = 0;
	now[i].b = 0;
  }	  
  n = 0;

  for (i = m[x] - 1; i >= 1; i--)
  {
	if (strcmp(p[x][i], "NOP;") == 0)
    {
	}
	else
	{
	  j = 0;
	  while (isdigit(p[x][i][j]) == 0)
		j++;
	  ss = p[x][i] + j;
      th1 = atof(ss);
	  th2 = 1 - th1;
	  if (strstr(p[x][i], ">") != 0)
	    swap(th1, th2);
	  ss = strstr(p[x][i], "PROC");
	  if (ss != 0)
	  {
		ss += 4;
		while (*ss == ' ')
		  ss++;
        ss[strlen(ss) - 1] = '\0';		
		for (k = 1; k <= N; k++)
		  if (strcmp(name[k], ss) == 0)
		  {
			if (fabs(t[k] - (- 1)) < zero)
			{
			  repeat = true;
			  return;
			}
//			for (j = 0; j <= now[0].a; j++)
//			  now[j].b *= th2;
			now[0].b += th1 * t[k];
			break;
		  }
      }
	  else
	  {
        ss = strstr(p[x][i], "GOTO") + 4;
 		while (*ss == ' ')
		  ss++;
        ss[strlen(ss) - 1] = '\0';	      
		j = atoi(ss);
  	    for (k = 0; k <= now[0].a; k++)
		  now[k].b *= th2;
		if (j > i)
		{
		  for (k = 0; k <= now[0].a; k++)
			now[k].b += th1 * d[j][k].b;
		}
	    else
		{
		  for (k = 1; k <= now[0].a; k++)
			if (now[k].a == j)
			  break;
		  if (now[k].a != j)
		  {
			now[++now[0].a].a = j;
			k = now[0].a;
		  }
		  now[k].b += th1;
		}
	  }
    }

	now[0].b++;

	for (j = 0; j <= now[0].a; j++)
	{
	  d[i][j].a = now[j].a;
	  d[i][j].b = now[j].b;
	}

	for (j = 1; j <= now[0].a; j++)
	  if (now[j].a == i)
	  {
		n++;
		for (k = 1; k <= now[0].a; k++)
		{
		  e[n][k].a = now[k].a;
		  e[n][k].b = now[k].b;
		}
		e[n][0].b = - now[0].b;
		e[n][j].b--;
		break;
	  }
  }

  for (k = 1; k <= n - 1; k++)
  {
	for (i = k; i <= n; i++)
	  if (fabs(e[i][k].b) > zero)
		break;
	if (i != k)
	  swap1(e[i], e[k]);
    for (i = k + 1; i <= n; i++)
	{
	  s = e[i][k].b / e[k][k].b;
	  for (j = k; j <= n; j++)
		e[i][j].b -= s * e[k][j].b;
	  e[i][0].b -= s * e[k][0].b;
    }
  }
  for (k = n; k >= 2; k--)
  {
	e[k][0].b /= e[k][k].b;
	e[k][k].b = 1;
	for (i = k - 1; i >= 1; i--)
	{
	  e[i][0].b -= e[i][k].b * e[k][0].b;
	  e[i][k].b = 0;
	}
  }
  e[1][0].b /= e[1][1].b;
  e[1][1].b = 1;
  t[x] = d[1][0].b;
  for (i = 1; i <= d[1][0].a; i++)
	t[x] += d[1][i].b * e[i][0].b;
}

int main()
{
  int i, j;
  char s[MAXN], *ss;

  ifstream cin("random.in", ios::in);

  cin.getline(s, MAXN, '\n');
  N = 0;
  while (1)
  {
    cin.getline(s, MAXN, '\n');
	if (strcmp(s, "PROG_END") == 0)
	  break;
	N++;
	m[N] = 0;
	ss = strstr(s, "PROC") + 5;
	while (*ss == ' ')
	  ss++;
	while (*(ss + strlen(ss) - 1) == ' ')
	  *(ss + strlen(ss) - 1) = '\0';
	strcpy(name[N], ss);
	while (1)
	{
	  cin.getline(s, MAXN, '\n');
	  ++m[N];
	  strcpy(p[N][m[N]], s);
	  if (strcmp(s, "END;") == 0)
		break;
	}
  }

	for (i = 1; i <= N; i++)
	  t[i] = - 1;

	i = N;
	while (i > 0)
	{
	  for (j = 1; j <= N; j++)
	  {
	    repeat = false;
	    calc(j);
		if (! repeat)
		  i--;
	  }
	}

	while (1)
	{
	  cin.getline(s, MAXN, '\n');
	  if (strcmp(s, "REQUEST_END") == 0)
		break;
	  for (i = 1; i <= N; i++)
		if (strcmp(s, name[i]) == 0)
		{
		  cout<<setprecision(3)<<setiosflags(ios::fixed)<<t[i]<<endl;
		  break;
		}
	}

  
  cin.close();
  system("pause");
  return 0;
}