// POJ 1515 - Street Directions
// Model: gpt-6-astra
// Orient DFS tree edges downward and back edges upward. Only bridges need
// both directions: low[child] > discovery[parent]. This is optimal because
// deleting a bridge disconnects the city. Any valid output order is allowed.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int v, id; Edge(int a,int b):v(a),id(b){} };
vector<Edge> adj[1005];
int disc[1005],low[1005],timer;
void dfs(int u,int parentEdge) {
    disc[u]=low[u]=++timer;
    for (int i=0;i<(int)adj[u].size();++i) {
        int v=adj[u][i].v, id=adj[u][i].id;
        if(id==parentEdge) continue;
        if(!disc[v]) {
            printf("%d %d\n",u,v);
            dfs(v,id);
            low[u]=min(low[u],low[v]);
            if(low[v]>disc[u]) printf("%d %d\n",v,u);
        } else if(disc[v]<disc[u]) {
            printf("%d %d\n",u,v);
            low[u]=min(low[u],disc[v]);
        }
    }
}
int main() {
    int n,m,tc=0;
    while(scanf("%d%d",&n,&m)==2 && (n||m)) {
        for(int i=1;i<=n;++i) { adj[i].clear(); disc[i]=0; }
        timer=0;
        for(int i=0;i<m;++i) {
            int u,v; scanf("%d%d",&u,&v);
            adj[u].push_back(Edge(v,i)); adj[v].push_back(Edge(u,i));
        }
        printf("%d\n\n",++tc);
        for(int i=1;i<=n;++i) if(!disc[i]) dfs(i,-1);
        puts("#");
    }
    return 0;
}
