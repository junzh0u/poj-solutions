// POJ 3567 - Cactus Reloaded
// Model: gpt-6-astra
// Iterative DFS splits the cactus into bridges and ordered cycles.
// Bottom-up heights combine at cut vertices; a sliding maximum finds
// the best pair on each cycle using its shorter arc. O(n + edges).
// Input paths describe edges, not necessarily simple paths.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    vector<vector<int> > g(n), cycles;
    for(int z=0;z<m;++z) {
        int k,u,v; scanf("%d%d",&k,&u); --u;
        for(int j=1;j<k;++j) {
            scanf("%d",&v); --v; g[u].push_back(v); g[v].push_back(u); u=v;
        }
    }
    vector<int> par(n,-1),dep(n,-1),it(n,0),order,st,cyclic(n,0);
    vector<vector<int> > rooted(n);
    dep[0]=0; st.push_back(0); order.push_back(0);
    while(!st.empty()) {
        int u=st.back();
        if(it[u]==(int)g[u].size()) { st.pop_back(); continue; }
        int v=g[u][it[u]++];
        if(v==par[u]) continue;
        if(dep[v]<0) {
            par[v]=u; dep[v]=dep[u]+1; order.push_back(v); st.push_back(v);
        } else if(dep[v]<dep[u]) {
            vector<int> c; c.push_back(v);
            for(int x=u;x!=v;x=par[x]) { c.push_back(x); cyclic[x]=1; }
            rooted[v].push_back((int)cycles.size()); cycles.push_back(c);
        }
    }
    vector<int> h(n,0);
    int ans=0;
    for(int oi=n-1;oi>=0;--oi) {
        int u=order[oi];
        for(int j=0;j<(int)g[u].size();++j) {
            int v=g[u][j];
            if(par[v]==u && !cyclic[v]) {
                ans=max(ans,h[u]+h[v]+1); h[u]=max(h[u],h[v]+1);
            }
        }
        for(int z=0;z<(int)rooted[u].size();++z) {
            const vector<int>& c=cycles[rooted[u][z]];
            int l=(int)c.size(),half=l/2;
            vector<int> q(2*l); int head=0,tail=0,best=0;
            // c[0]'s current height belongs to already processed blocks.
            for(int j=0;j<2*l;++j) {
                while(head<tail && q[head]<j-half) ++head;
                if(head<tail) ans=max(ans,h[c[j%l]]+j+h[c[q[head]%l]]-q[head]);
                while(head<tail && h[c[q[tail-1]%l]]-q[tail-1]<=h[c[j%l]]-j) --tail;
                q[tail++]=j;
            }
            for(int j=1;j<l;++j) best=max(best,h[c[j]]+min(j,l-j));
            h[u]=max(h[u],best);
        }
    }
    printf("%d\n",ans);
    return 0;
}
