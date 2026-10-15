// POJ 3611 - Minimum Weighted Perfect Fractional b-Matching
// Model: gpt-6.1-sol
// Split every vertex into left and right copies. Replace each undirected
// edge by both directed left-to-right arcs, with its capacity and cost.
// A minimum-cost flow supplies b(v) on each side. Averaging its two arc
// flows gives a feasible fractional matching of half the flow cost;
// conversely every matching gives such a flow, so the optimum is exact.
// Fractional answers are printed exactly as integers or half-integers.
#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
#include <algorithm>
using namespace std;
struct Edge { int to, rev, cap, cost; Edge(int t,int r,int a,int c):to(t),rev(r),cap(a),cost(c){} };
vector<Edge> g[205];
void add(int a,int b,int cap,int cost) {
    int x=g[a].size(), y=g[b].size();
    g[a].push_back(Edge(b,y,cap,cost));
    g[b].push_back(Edge(a,x,0,-cost));
}
int main() {
    int m,n;
    if(scanf("%d%d",&m,&n)!=2) return 0;
    int s=0,t=2*n+1,N=t+1;
    for(int i=0;i<m;++i) {
        int x,y,u,c; scanf("%d%d%d%d",&x,&y,&u,&c);
        add(x,n+y,u,c); add(y,n+x,u,c);
    }
    int need=0;
    for(int i=1;i<=n;++i) { int b; scanf("%d",&b); need+=b; add(s,i,b,0); add(n+i,t,b,0); }
    int h[205]={0},d[205],pv[205],pe[205],flow=0,cost=0;
    const int INF=1000000000;
    while(flow<need) {
        fill(d,d+N,INF); d[s]=0;
        priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > q;
        q.push(make_pair(0,s));
        while(!q.empty()) {
            int v=q.top().second,dist=q.top().first; q.pop();
            if(dist!=d[v]) continue;
            for(int i=0;i<(int)g[v].size();++i) {
                Edge &e=g[v][i];
                int nd=dist+e.cost+h[v]-h[e.to];
                if(e.cap>0 && nd<d[e.to]) { d[e.to]=nd; pv[e.to]=v; pe[e.to]=i; q.push(make_pair(nd,e.to)); }
            }
        }
        if(d[t]==INF) break;
        for(int i=0;i<N;++i) if(d[i]<INF) h[i]+=d[i];
        int a=need-flow;
        for(int v=t;v!=s;v=pv[v]) a=min(a,g[pv[v]][pe[v]].cap);
        for(int v=t;v!=s;v=pv[v]) { Edge &e=g[pv[v]][pe[v]]; cost+=a*e.cost; e.cap-=a; g[v][e.rev].cap+=a; }
        flow+=a;
    }
    if(cost%2) printf("%d.5\n",cost/2); else printf("%d\n",cost/2);
    return 0;
}
