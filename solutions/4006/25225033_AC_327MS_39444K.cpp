// POJ 4006 - Genghis Khan the Conqueror
// Model: gpt-6.1-sol
// Prim builds one MST. For every tree-edge cut, subtree minima over
// all outside vertices find its cheapest non-tree replacement in O(N^2).
// Each independent increase either leaves this MST unchanged or replaces
// one tree edge by the cheaper of its new cost and the cut replacement.
// Input ends at 0 0, as shown by the sample; updates are independent.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=2000000000;
int n, timer;
vector<int> g, par, cost, tin, tout, order;
vector< vector<int> > child;
void dfs(int u) {
    tin[u]=timer++;
    order.push_back(u);
    for (int k=0;k<(int)child[u].size();++k) dfs(child[u][k]);
    tout[u]=timer;
}
int main() {
    int m;
    while (scanf("%d%d",&n,&m)==2 && n) {
        g.assign(n*n,INF);
        for (int i=0;i<m;++i) {
            int x,y,c; scanf("%d%d%d",&x,&y,&c);
            g[x*n+y]=g[y*n+x]=c;
        }
        par.assign(n,-1); cost.assign(n,INF);
        vector<int> used(n,0); cost[0]=0;
        int total=0;
        for (int k=0;k<n;++k) {
            int u=-1;
            for (int v=0;v<n;++v)
                if (!used[v] && (u<0 || cost[v]<cost[u])) u=v;
            used[u]=1; total+=cost[u];
            for (int v=0;v<n;++v)
                if (!used[v] && g[u*n+v]<cost[v]) {
                    cost[v]=g[u*n+v]; par[v]=u;
                }
        }
        child.assign(n,vector<int>());
        for (int v=1;v<n;++v) child[par[v]].push_back(v);
        tin.resize(n); tout.resize(n); order.clear(); timer=0; dfs(0);
        vector<int> replacement(n,INF), best(n);
        for (int r=0;r<n;++r) {
            for (int v=0;v<n;++v)
                best[v]=(v==r || par[v]==r || par[r]==v) ? INF : g[r*n+v];
            for (int k=n-1;k>0;--k) {
                int v=order[k];
                if (!(tin[v]<=tin[r] && tin[r]<tout[v]))
                    replacement[v]=min(replacement[v],best[v]);
                best[par[v]]=min(best[par[v]],best[v]);
            }
        }
        int q; scanf("%d",&q);
        double sum=0;
        for (int k=0;k<q;++k) {
            int x,y,c; scanf("%d%d%d",&x,&y,&c);
            int v=-1;
            if (par[x]==y) v=x;
            if (par[y]==x) v=y;
            int answer=total;
            if (v>=0) answer+=min(c,replacement[v])-cost[v];
            sum+=answer;
        }
        printf("%.4f\n",sum/q);
    }
    return 0;
}
