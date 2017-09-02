#include <iostream>
#include <fstream>
#include <cmath>
#include <cctype>
#define zero 1e-8
#define MAXN 110
using namespace std;

double t[MAXN];
char code[MAXN][MAXN][MAXN], name[100][100], del[300], *tok;
int m[MAXN], N, n;
double state[MAXN][MAXN], A[MAXN][MAXN + 1], r;

void Gauss()
{
  int i, j, k;

  for (j = 1; j <= n; j++)
  {
	for (i = j; i <= n; i++)
	  if (!(fabs(A[i][j]) < zero))
		break;

	if (i != j)
	  for (k = 1; k <= n + 1; k++)
		swap(A[i][k], A[j][k]);
	r = A[j][j];
	for (k = j; k <= n + 1; k++)
	  A[j][k] /= r;
	for (i = j + 1; i <= n; i++)
    {
	  r = A[i][j];
	  for (k = j; k <= n + 1; k++)
	    A[i][k] -= r * A[j][k];
	}
  }
  for (j = n; j >= 1; j--)
  {
	for (i = j - 1; i >= 1; i--)
	{
	  r = A[i][j];
	  for (k = 1; k <= n + 1; k++)
		A[i][k] -= A[j][k] * r;
	}
  }
}

void IntoA(int x)
{
  int i;
  for (i = 1; i <= n; i++)
	A[x][i] = state[x][i];
  A[x][n + 1] = - state[x][0];
  A[x][x]--;
}

bool calc(int x)
{
  double case1, case2;
  int i, j, k;
  char s[100];

  n = m[x];
  for (i = 0; i <= n; i++)
    state[n][i] = 0;
  IntoA(n);

  for (k = n - 1; k >= 1; k--)
  {
	for (i = 0; i <= n; i++)
	  state[k][i] = state[k + 1][i];

    if (strstr(code[x][k], "NOP;") != 0)
	  state[k][0]++;
	else
	{
	  strcpy(s, code[x][k]);
      tok = strtok(s, del);
	  tok = strtok(NULL, del);
	  tok = strtok(NULL, del);
	  case1 = atof(tok);
	  case2 = 1 - case1;
	  if (strstr(code[x][k], ">") != 0)
		swap(case1, case2);
	  tok = strtok(NULL, del);
	  tok = strtok(NULL, del);	

   	  if (strstr(code[x][k], "PROC") != 0)
	  {
	    for (i = 1; i <= N; i++)
		  if (strcmp(name[i], tok) == 0)
		    break;
	    if (fabs(t[i]) < zero)
		  return false;
	    state[k][0] += case1 * t[i];
		state[k][0]++;
	  }
	  else
	  {
        j = atoi(tok);
        for (i = 0; i <= n; i++)
  		  state[k][i] *= case2;
		if (j > k)
		  for (i = 0; i <= n; i++)
			state[k][i] += case1 * state[j][i];
		else
          state[k][j] += case1;
		state[k][0]++;
	  }
	}

    IntoA(k);
  }

  return true;
}

int main()
{
  int i, j, ndel;
  char s[100];

  ndel = 0;
  for (i = 255; i >= 0; i--)
	if (! (isalpha(char(i)) || isdigit(char(i)) || char(i) == '.'))
	  del[++ndel - 1] = char(i);
  del[ndel] = '\0';

  ifstream cin("random.dat", ios::in);

  cin.getline(s, 200, '\n');
  N = 0;
  while (1)
  {
    cin.getline(s, 100, '\n');
	if (strcmp(s, "PROG_END") == 0)
	  break;
	N++;
	m[N] = 0;
    tok = strtok(s, del);
	tok = strtok(NULL, del);
	strcpy(name[N], tok);
	while (1)
	{
	  cin.getline(s, 100, '\n');
	  ++m[N];
	  strcpy(code[N][m[N]], s);
	  if (strcmp(s, "END;") == 0)
		break;
	}
  }

	for (i = 1; i <= N; i++)
	  t[i] = 0;

	i = N;
	while (i > 0)
	{
	  for (j = 1; j <= N; j++)
		if (fabs(t[j]) < zero)
  	      if (calc(j))
		  {
	   	    i--;
			Gauss();
            t[j] = A[1][n + 1];
		  }
	}

	while (1)
	{
	  cin.getline(s, 100, '\n');
	  if (strcmp(s, "REQUEST_END") == 0)
		break;
	  for (i = 1; i <= N; i++)
		if (strcmp(s, name[i]) == 0)
		{
		  printf("%.3Lf\n", t[i]);
		  break;
		}
	}
  
  cin.close();
  system("pause");
  return 0;
}