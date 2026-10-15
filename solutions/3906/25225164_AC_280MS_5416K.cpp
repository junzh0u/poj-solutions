// POJ 3906 - Lucky cities
// Model: gpt-6.1-sol
// Every vertex of a non-bipartite vertex-biconnected block lies on an
// odd simple cycle. Extract blocks with an iterative Tarjan DFS and mark
// their endpoints when a block contains a same-depth-parity edge.
// Articulation points do not spread luck to neighboring bipartite blocks.
// O(N+M) time and space; explicit DFS frames avoid a deep call stack.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int v, next; Edge(int a,int b):v(a),next(b){} };
int main() {
    int T; if(scanf("%d", &T)!=1) return 0;
    while(T--) {
        int n,m; scanf("%d%d",&n,&m);
        vector<int> head(n,-1); vector<Edge> edges; edges.reserve(2*m);
        for(int i=0;i<m;++i) {
            int a,b; scanf("%d%d",&a,&b); --a; --b;
            edges.push_back(Edge(b,head[a])); head[a]=2*i;
            edges.push_back(Edge(a,head[b])); head[b]=2*i+1;
        }
        vector<int> num(n,0),low(n),depth(n),parent(n,-1),iter(n);
        vector<int> dfs, stack, block; vector<char> lucky(n,0);
        dfs.reserve(n); stack.reserve(m); block.reserve(m);
        int clock=0;
        for(int root=0;root<n;++root) if(!num[root]) {
            num[root]=low[root]=++clock; depth[root]=0;
            iter[root]=head[root]; dfs.push_back(root);
            while(!dfs.empty()) {
                int u=dfs.back(), e=iter[u];
                if(e!=-1) {
                    iter[u]=edges[e].next;
                    if(parent[u]!=-1 && e==(parent[u]^1)) continue;
                    int v=edges[e].v;
                    if(!num[v]) {
                        stack.push_back(e); parent[v]=e; depth[v]=depth[u]+1;
                        num[v]=low[v]=++clock; iter[v]=head[v]; dfs.push_back(v);
                    } else if(num[v]<num[u]) {
                        stack.push_back(e); low[u]=min(low[u],num[v]);
                    }
                } else {
                    dfs.pop_back();
                    if(parent[u]==-1) continue;
                    int pe=parent[u], p=edges[pe^1].v;
                    low[p]=min(low[p],low[u]);
                    if(low[u]>=num[p]) {
                        block.clear(); bool odd=false; int x;
                        do {
                            x=stack.back(); stack.pop_back(); block.push_back(x);
                            int a=edges[x^1].v,b=edges[x].v;
                            if((depth[a]&1)==(depth[b]&1)) odd=true;
                        } while(x!=pe);
                        if(odd) for(size_t j=0;j<block.size();++j) {
                            lucky[edges[block[j]].v]=1;
                            lucky[edges[block[j]^1].v]=1;
                        }
                    }
                }
            }
        }
        int answer=0; for(int i=0;i<n;++i) answer+=lucky[i];
        printf("%d\n",answer);
    }
    return 0;
}
