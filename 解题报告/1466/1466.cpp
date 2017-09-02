const int maxn = 500;
int map[maxn][maxn],t[maxn],p[maxn];
bool isVis[maxn];
int isGirl[maxn];

#include <iostream.h>
#include <memory.h>

void readdata(int n){
	int i,j,s;
	char c;
	for (i = 0;i < n; i++)
	{
		cin >> s;
		while (cin >> c,c != '(');
		cin>>t[s];
		while (cin>>c, c != ')');
		for (j = 0; j < t[s]; j ++)
			cin >>map[s][j];
	}
}


void color(int k)//分出二分图的节点
{
	int i;
	for (i = 0;i < t[k];i ++)
		if (isGirl[map[k][i]] == 0)
		{
			isGirl[map[k][i]] = 3 - isGirl[k];
			color(map[k][i]);
		}

}

bool Find(int k)
{
	int i,j;
	isVis[k] = true;
	for (i = 0;i < t[k];i ++)
		if (p[map[k][i]] < 0)
		{
			p[map[k][i]] = k;
			return true;
		}
		else
			if (!isVis[p[map[k][i]]])
			{
				j = p[map[k][i]];
				p[map[k][i]] = k;
				if (Find(j)) return true;
				p[map[k][i]] = j;
			}
		return false;
}

int main(){


	int n,i,ans;

	while (cin >> n){
		readdata(n);

		memset(isGirl,0,sizeof(isGirl));
		for (i = 0;i < n;i ++)
			if (isGirl[i] == 0) 
			{
				isGirl[i] = 2;
				color(i);
			}

		ans = n;
		memset(p,-1,sizeof(p));
		for (i = 0;i < n;i ++)
		    if (isGirl[i] == 1)
			{
				memset(isVis,false,sizeof(isVis));				
				if (Find(i)) ans --;
			}
		cout<<ans<<endl;
	}
	return 0;
}