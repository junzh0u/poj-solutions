// POJ 3834 - Graph Game
// Model: gpt-6.1-sol
// Lehman's theorem reduces the second player's win to two edge-disjoint
// spanning trees. The Nash-Williams-Tutte condition checks every vertex
// partition: at least twice (number of blocks minus one) crossing edges.
// Subset DP maximizes sum of (internal edges + 2) over all partitions.
// Parallel edges count separately; self-loops cancel from the inequality.
#include <cstdio>
#include <algorithm>
using namespace std;
int a[10][10], inside[1024], dp[1024];
int main() {
    int n,m;
    while (scanf("%d%d", &n,&m)==2 && n!=-1) {
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) a[i][j]=0;
        for(int i=0;i<m;++i) { int u,v; scanf("%d%d",&u,&v); if(u>v) swap(u,v); ++a[u][v]; }
        int lim=1<<n; inside[0]=0; dp[0]=0;
        for(int mask=1;mask<lim;++mask) {
            int v=0; while(!(mask&(1<<v))) ++v;
            int rest=mask^(1<<v);
            inside[mask]=inside[rest]+a[v][v];
            for(int u=v+1;u<n;++u) if(rest&(1<<u)) inside[mask]+=a[v][u];
            dp[mask]=0;
            for(int sub=mask;sub;sub=(sub-1)&mask) if(sub&(1<<v))
                dp[mask]=max(dp[mask],dp[mask^sub]+inside[sub]+2);
        }
        puts(dp[lim-1]<=m+2 ? "YES" : "NO");
    }
    return 0;
}
