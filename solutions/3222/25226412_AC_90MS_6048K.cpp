// POJ 3222 - Edge Pairing
// Model: gpt-6-astra
// Build a spanning forest iteratively and assign each non-tree edge once.
// Process children before parents, pairing incident edges locally; an odd
// leftover uses the parent edge. A root leftover means its component is odd.
// Edge IDs preserve parallel edges; all components are handled because the
// discussion reports input that violates the stated connectedness guarantee.
// Time and memory: O(n + m). No recursive calls are needed.
#include <cstdio>
#include <vector>
#include <utility>
using namespace std;
struct Triple { int a,b,c; Triple(int x,int y,int z):a(x),b(y),c(z){} };
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    vector<int> a(m),b(m),parent(n+1,-1),pe(n+1,-1),order;
    vector<vector<pair<int,int> > > adj(n+1);
    vector<vector<int> > pending(n+1);
    vector<char> tree(m,0);
    for(int i=0;i<m;++i) {
        scanf("%d%d",&a[i],&b[i]);
        adj[a[i]].push_back(make_pair(b[i],i));
        adj[b[i]].push_back(make_pair(a[i],i));
    }
    for(int root=1;root<=n;++root) if(parent[root]==-1) {
        parent[root]=0;
        int start=(int)order.size(); order.push_back(root);
        for(int k=start;k<(int)order.size();++k) {
            int v=order[k];
            for(int j=0;j<(int)adj[v].size();++j) {
                int w=adj[v][j].first, e=adj[v][j].second;
                if(parent[w]==-1) {
                    parent[w]=v; pe[w]=e; tree[e]=1; order.push_back(w);
                }
            }
        }
    }
    for(int e=0;e<m;++e) if(!tree[e]) pending[a[e]].push_back(b[e]);
    vector<Triple> ans;
    for(int k=(int)order.size()-1;k>=0;--k) {
        int v=order[k];
        vector<int>& p=pending[v];
        int j=0;
        for(;j+1<(int)p.size();j+=2) ans.push_back(Triple(p[j],v,p[j+1]));
        if(j<(int)p.size()) {
            if(parent[v]==0) { puts("NO"); return 0; }
            ans.push_back(Triple(p[j],v,parent[v]));
        } else if(parent[v]) pending[parent[v]].push_back(v);
    }
    for(int i=0;i<(int)ans.size();++i)
        printf("%d %d %d\n",ans[i].a,ans[i].b,ans[i].c);
    return 0;
}
