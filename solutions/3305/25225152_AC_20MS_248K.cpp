// POJ 3305 - Surveillance
// Model: gpt-6.1-sol
// Subdivide each passage into a node. Both camera types then cover exactly
// the nodes at distance at most two. Greedily cover the deepest uncovered
// node by placing a camera at its second ancestor (or the component root).
// Moving any camera covering that node up to this ancestor cannot lose an
// uncovered node of no greater depth, so each greedy choice is optimal.
// Process every component: the discussion reports possibly disconnected data.
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
vector<int> g[305];
int par[305],dep[305];
bool covered[305];
void dfs(int u,int p) {
    par[u]=p;
    for(size_t i=0;i<g[u].size();++i) {
        int v=g[u][i];
        if(par[v]!=-1) continue;
        dep[v]=dep[u]+1; dfs(v,u);
    }
}
void cover(int u,int p,int d) {
    covered[u]=true;
    if(d==0) return;
    for(size_t i=0;i<g[u].size();++i)
        if(g[u][i]!=p) cover(g[u][i],u,d-1);
}
bool deeper(int a,int b) { return dep[a]>dep[b]; }
int main() {
    int T; cin>>T;
    while(T--) {
        int P; cin>>P;
        for(int i=0;i<305;++i) { g[i].clear(); par[i]=-1; dep[i]=0; covered[i]=false; }
        map<int,int> ids; int n=0;
        for(int i=0;i<P;++i) {
            int a,b; cin>>a>>b;
            if(ids.find(a)==ids.end()) ids[a]=n++;
            if(ids.find(b)==ids.end()) ids[b]=n++;
            int u=ids[a],v=ids[b],e=n++;
            g[u].push_back(e);g[e].push_back(u);
            g[v].push_back(e);g[e].push_back(v);
        }
        for(int i=0;i<n;++i) if(par[i]==-1) dfs(i,i);
        vector<int> order;
        for(int i=0;i<n;++i) order.push_back(i);
        sort(order.begin(),order.end(),deeper);
        int ans=0;
        for(int i=0;i<n;++i) if(!covered[order[i]]) {
            int u=par[par[order[i]]]; cover(u,-1,2); ++ans;
        }
        cout<<ans<<'\n';
    }
    return 0;
}
