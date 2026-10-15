// POJ 2768 - Pesky Heroes
// Model: gpt-6.1-sol
// Bottom-up tree DP. A normal left-turn walk tours the whole subtree;
// its single right turn can omit one child subtree. Safe fringe branches
// can share a gateway until traps occur in both children. Only original
// dead ends reachable from the entrance without crossing traps matter.
// Iterative traversal avoids recursion depth on a 50000-vertex chain.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int n,m;
    while (scanf("%d%d", &n,&m)==2 && n) {
        vector< vector<int> > adj(n);
        for(int i=0;i<n;++i) {
            int k; scanf("%d",&k);
            while(k--) { int v; scanf("%d",&v); adj[i].push_back(v-1); }
        }
        vector<int> trap(n,0),parent(n,-1),order,dp(n,0),clean(n,0),open(n,0);
        for(int i=0;i<m;++i) { int v; scanf("%d",&v); trap[v-1]=1; }
        order.push_back(0);
        for(unsigned i=0;i<order.size();++i) {
            int u=order[i];
            if(trap[u]) continue;
            for(unsigned j=0;j<adj[u].size();++j) {
                int v=adj[u][j]; if(v==parent[u]) continue;
                parent[v]=u; order.push_back(v);
            }
        }
        for(int i=(int)order.size()-1;i>=0;--i) {
            int u=order[i]; if(trap[u]) continue;
            int ch[2],k=0;
            for(unsigned j=0;j<adj[u].size();++j)
                if(adj[u][j]!=parent[u]) ch[k++]=adj[u][j];
            if(k==0) {clean[u]=open[u]=1; dp[u]=1;}
            else if(k==1) {int v=ch[0]; clean[u]=clean[v];open[u]=open[v];dp[u]=dp[v];}
            else {
                int a=ch[0],b=ch[1];
                if(clean[a] && clean[b]) {clean[u]=open[u]=1;dp[u]=1;}
                else if(clean[a] || clean[b]) {
                    int v=clean[a]?b:a;
                    open[u]=1; dp[u]=dp[v]+(open[v]?0:1);
                } else dp[u]=dp[a]+dp[b];
            }
        }
        printf("%d\n",dp[0]);
    }
    return 0;
}
