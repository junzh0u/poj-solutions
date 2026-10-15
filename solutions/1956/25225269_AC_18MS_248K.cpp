// POJ 1956 - Pumps and Pipes
// Model: gpt-6.1-sol
// Build feasible stretches between pump joints, then find a shortest path.
// Ascending reconstruction chooses the lexicographically earliest placement.
// All pressures use integer millibars: a slope s loses s millibars per meter.
// A pump cannot reduce pressure; its inlet must therefore be in [2000,8000].
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int cases; scanf("%d", &cases);
    const int loss[6] = {1,3,6,10,15,20};
    for (int tc=1; tc<=cases; ++tc) {
        int f,n,m; scanf("%d%d%d", &f,&n,&m);
        int delta[401], length=0;
        for (int k=0;k<m;++k) {
            int l,s; scanf("%d%d",&l,&s);
            while (l--) delta[++length]=s+loss[f/200-1];
        }
        bool edge[21][21] = {{false}};
        for (int i=0;i<n;++i) {
            int p=8000;
            for (int x=i*20+1;x<=n*20;++x) {
                p-=delta[x];
                if (p<2000 || p>12000) break;
                if (x%20==0) {
                    int j=x/20;
                    edge[i][j]=(j==n ? p>=5000 && p<=8000 : p<=8000);
                }
            }
        }
        int dp[21]; dp[n]=0;
        for (int i=n-1;i>=0;--i) {
            dp[i]=100;
            for (int j=i+1;j<=n;++j)
                if (edge[i][j]) dp[i]=min(dp[i],1+dp[j]);
        }
        printf("Scenario #%d:\n",tc);
        if (dp[0]>=100) puts("no solution");
        else {
            printf("%d: 0",dp[0]);
            int i=0;
            while (i<n) {
                int j=i+1;
                while (!(edge[i][j] && dp[i]==dp[j]+1)) ++j;
                if (j<n) printf(",%d",j);
                i=j;
            }
            puts("");
        }
        puts("");
    }
    return 0;
}
