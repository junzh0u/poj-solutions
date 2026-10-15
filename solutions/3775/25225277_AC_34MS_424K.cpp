// POJ 3775 - Chase
// Model: gpt-6.1-sol
// Dijkstra gives the unique shortest-path tree. A tree knapsack maximizes
// conditional capture probability for each exact agent budget. Children are
// weighted by valid roads, then local capture is combined with survival.
// No monotonicity of the probability table is assumed; all P agents are used.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=1000000000;
int n,p,w[105][105],cnt[105][105],d[105];
vector<int> child[105];
double pt[105][55],dp[105][55];
void solve(int u) {
    double a[55],b[55];
    for(int k=0;k<=p;++k) a[k]=(k==0?0:-1e100);
    int total=0;
    for(unsigned t=0;t<child[u].size();++t) total+=cnt[u][child[u][t]];
    for(unsigned t=0;t<child[u].size();++t) {
        int v=child[u][t]; solve(v);
        for(int k=0;k<=p;++k) {
            b[k]=-1e100;
            for(int j=0;j<=k;++j)
                b[k]=max(b[k],a[k-j]+dp[v][j]*cnt[u][v]/total);
        }
        for(int k=0;k<=p;++k) a[k]=b[k];
    }
    for(int k=0;k<=p;++k) {
        dp[u][k]=0;
        for(int j=0;j<=k;++j) if(a[k-j]>=0)
            dp[u][k]=max(dp[u][k],pt[u][j]+(1-pt[u][j])*a[k-j]);
    }
}
int main() {
    int m;
    while(scanf("%d%d",&n,&m)==2 && (n||m)) {
        for(int i=0;i<n;++i) {
            child[i].clear();
            for(int j=0;j<n;++j) { w[i][j]=INF; cnt[i][j]=0; }
        }
        for(int i=0;i<m;++i) {
            int a,b,c; scanf("%d%d%d",&a,&b,&c);
            if(a==b) continue;
            if(c<w[a][b]) { w[a][b]=w[b][a]=c; cnt[a][b]=cnt[b][a]=1; }
            else if(c==w[a][b]) { ++cnt[a][b]; ++cnt[b][a]; }
        }
        scanf("%d",&p);
        for(int i=0;i<n;++i) {
            pt[i][0]=0;
            for(int j=1;j<=p;++j) scanf("%lf",&pt[i][j]);
        }
        bool used[105]={false};
        for(int i=0;i<n;++i) d[i]=INF;
        d[0]=0;
        for(int t=0;t<n;++t) {
            int u=-1;
            for(int i=0;i<n;++i) if(!used[i] && (u<0 || d[i]<d[u])) u=i;
            if(u<0 || d[u]==INF) break;
            used[u]=true;
            for(int v=0;v<n;++v) if(w[u][v]<INF)
                d[v]=min(d[v],d[u]+w[u][v]);
        }
        bool unreachable=false;
        for(int u=0;u<n;++u) {
            if(d[u]==INF) { unreachable=true; continue; }
            for(int v=0;v<n;++v) if(w[u][v]<INF && d[u]+w[u][v]==d[v])
                child[u].push_back(v);
        }
        solve(0);
        double ans=dp[0][p];
        if(unreachable) for(int k=0;k<p;++k) ans=max(ans,dp[0][k]);
        printf("%.2f\n",100*ans);
    }
    return 0;
}
