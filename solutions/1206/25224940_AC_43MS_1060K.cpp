// POJ 1206 - Servers
// Model: gpt-6.1-sol
// Process ranks from high to low. The nearest strictly higher-rank server
// excludes a candidate at equal or greater distance. Truncated Dijkstra
// enumerates interesting pairs; every prefix of a surviving shortest path
// also survives, so pruning cannot hide an interesting destination.
// Equal ranks are inserted into the distance bound only after the whole group.
#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
using namespace std;
const int INF = 1000000000;
struct Edge { int v, w; Edge(int a, int b):v(a),w(b){} };
typedef pair<int,int> State;
typedef priority_queue<State, vector<State>, greater<State> > Heap;
int main() {
    int n,m;
    if (scanf("%d%d",&n,&m)!=2) return 0;
    vector<vector<Edge> > g(n);
    vector<int> group[11];
    for(int i=0,r;i<n;++i) { scanf("%d",&r); group[r].push_back(i); }
    for(int i=0,a,b,w;i<m;++i) {
        scanf("%d%d%d",&a,&b,&w); --a;--b;
        g[a].push_back(Edge(b,w)); g[b].push_back(Edge(a,w));
    }
    vector<int> bound(n,INF), dist(n), seen(n,0);
    int stamp=0, answer=0;
    for(int r=10;r>=1;--r) {
        for(size_t k=0;k<group[r].size();++k) {
            int s=group[r][k]; ++stamp;
            Heap q; dist[s]=0;seen[s]=stamp; q.push(State(0,s));
            while(!q.empty()) {
                State p=q.top();q.pop(); int d=p.first,u=p.second;
                if(d!=dist[u]) continue;
                ++answer;
                for(size_t j=0;j<g[u].size();++j) {
                    int v=g[u][j].v, nd=d+g[u][j].w;
                    if(nd<bound[v] && (seen[v]!=stamp || nd<dist[v])) {
                        seen[v]=stamp;dist[v]=nd;q.push(State(nd,v));
                    }
                }
            }
        }
        Heap q;
        for(size_t k=0;k<group[r].size();++k) {
            int s=group[r][k];bound[s]=0;q.push(State(0,s));
        }
        while(!q.empty()) {
            State p=q.top();q.pop();int d=p.first,u=p.second;
            if(d!=bound[u]) continue;
            for(size_t j=0;j<g[u].size();++j) {
                int v=g[u][j].v, nd=d+g[u][j].w;
                if(nd<bound[v]) { bound[v]=nd;q.push(State(nd,v)); }
            }
        }
    }
    printf("%d\n",answer);
    return 0;
}
