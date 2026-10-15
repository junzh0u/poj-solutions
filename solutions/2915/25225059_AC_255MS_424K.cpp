// POJ 2915 - Zuma
// Model: gpt-6.1-sol
// Run interval DP with Pareto frontiers of incomplete and exploding chains.
// Initial oversized runs require one touch, so normalize them to M-1 balls.
// Based on angeldust discussion 174670; checked against direct game search.
#include <cctype>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <string>
#include <string.h>
#include <queue>
#include <set>
using namespace std;
int p[26];
int colors[26][100];
pair<int,int> block[200];
int dp[200][200];
pair<int,int> mid[100][2][20];
int mid_sz[100][2];
void inline Add_mid0(int idx,int cnt,int cost)
{
	for(int i=0;i<mid_sz[idx][0];i++)
	{
		if(mid[idx][0][i].first<=cnt)
		{
			if(cost-cnt>=mid[idx][0][i].second-mid[idx][0][i].first) return;
		}
		else if(cost-cnt<=mid[idx][0][i].second-mid[idx][0][i].first) {swap(mid[idx][0][i],mid[idx][0][--mid_sz[idx][0]]); i--;}
	}
	mid[idx][0][mid_sz[idx][0]++]=make_pair(cnt,cost);
}
void inline Add_mid1(int idx,int cnt,int cost)
{
	for(int i=0;i<mid_sz[idx][1];i++)
	{
		if(mid[idx][1][i].first<=cnt)
		{
			if(cost>=mid[idx][1][i].second) return;
		}
		else if(mid[idx][1][i].second>=cost) {swap(mid[idx][1][i],mid[idx][1][--mid_sz[idx][1]]); i--;}
	}
	mid[idx][1][mid_sz[idx][1]++]=make_pair(cnt,cost);
}
int main()
{
	int m,n;
	char c,pre;
	while(scanf("%d",&m)!=EOF)
	{
		n=0;
		memset(p,0,26<<2);
		while((pre=getchar())&&!isupper(pre));
		while(1)
		{
			block[n]=make_pair(pre-'A',p[pre-'A']);
			dp[n][n]=1;
			while((c=getchar())&&c==pre) dp[n][n]++;
			if(dp[n][n]>=m) dp[n][n]=1;
			else dp[n][n]=m-dp[n][n];
			colors[pre-'A'][p[pre-'A']++]=n++;
			if(!isupper(c)) break;
			pre=c;
		}
		for(int i=1;i<n;i++)
		{
			for(int j=0;j<i;j++) dp[j][i]=dp[j][i-1]+dp[i][i];
			int ci=block[i].second;
			mid[ci][0][0]=make_pair(m-dp[i][i],0);
			mid_sz[ci][0]=1;
			mid_sz[ci][1]=0;
			for(int cj=ci-1,j;cj>=0;cj--)
			{
				j=colors[block[i].first][cj];
				mid_sz[cj][0]=mid_sz[cj][1]=0;
				for(int ck=ci,k;ck>cj;ck--)
				{
					k=colors[block[i].first][ck];
					for(int i_mid=0;i_mid<mid_sz[ck][0];i_mid++)
					{
						int cost=dp[j+1][k-1]+mid[ck][0][i_mid].second;
						if(cost>=dp[j][i]) continue;
						if(dp[j][j]>mid[ck][0][i_mid].first)
						{
							Add_mid0(cj,m-dp[j][j]+mid[ck][0][i_mid].first,cost);
							dp[j][i]=min(dp[j][i],cost+dp[j][j]-mid[ck][0][i_mid].first);
						}
						else
						{
							Add_mid1(cj,m-dp[j][j],cost);
							dp[j][i]=cost;
						}
					}
					for(int i_mid=0;i_mid<mid_sz[ck][1];i_mid++)
					{
						int cost=dp[j+1][k-1]+mid[ck][1][i_mid].second;
						if(cost>=dp[j][i]) continue;
						if(dp[j][j]>mid[ck][1][i_mid].first)
						{
							Add_mid1(cj,m-dp[j][j]+mid[ck][1][i_mid].first,cost);
							dp[j][i]=cost;
						}
					}
				}
				for(int k=j-1;k>=0;k--) dp[k][i]=min(dp[k][i],dp[k][j-1]+dp[j][i]);
			}
		}
		printf("%d\n",dp[0][n-1]);
	}
}

