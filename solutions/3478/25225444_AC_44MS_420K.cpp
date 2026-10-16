// POJ 3478 - Undergraduate Instruction Evaluation
// Model: gpt-6.1-sol
// Dualize the interval packing LP into minimum-cost interval coverage.
// Prefix-boundary flow and zero-cost backward arcs enforce each weight's
// coverage lower bound. The interval matrix is totally unimodular, so the
// integral minimum-cost flow equals the original optimum.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int to, rev, cap, cost; Edge(int t,int r,int a,int c):to(t),rev(r),cap(a),cost(c){} };
vector<Edge> g[205];
void add(int u,int v,int cap,int cost) {
    g[u].push_back(Edge(v,(int)g[v].size(),cap,cost));
    g[v].push_back(Edge(u,(int)g[u].size()-1,0,-cost));
}
int main() {
    int m,n; if(!(cin>>m>>n)) return 0;
    int x[4000],y[4000],b[202]={0};
    for(int i=0;i<m;++i) cin>>x[i]>>y[i];
    for(int i=1;i<=n;++i) cin>>b[i];
    const int INF=1000000000;
    for(int i=0,c;i<m;++i) { cin>>c; add(x[i]-1,y[i],INF,c); }
    int s=n+1,t=n+2,N=n+3,need=0;
    for(int i=1;i<=n;++i) add(i,i-1,INF,0);
    for(int i=0;i<=n;++i) {
        int d=b[i+1]-b[i];
        if(d>0) { add(s,i,d,0); need+=d; }
        if(d<0) add(i,t,-d,0);
    }
    long long h[205]={0},dist[205],ans=0;
    const long long BIG=1LL<<60;
    int pv[205],pe[205]; bool used[205];
    while(need) {
        fill(dist,dist+N,BIG); fill(used,used+N,false); dist[s]=0;
        for(int k=0;k<N;++k) {
            int u=-1;
            for(int v=0;v<N;++v) if(!used[v] && (u<0 || dist[v]<dist[u])) u=v;
            if(u<0 || dist[u]==BIG) break;
            used[u]=true;
            for(int j=0;j<(int)g[u].size();++j) {
                Edge &e=g[u][j];
                long long nd=dist[u]+e.cost+h[u]-h[e.to];
                if(e.cap && nd<dist[e.to]) { dist[e.to]=nd; pv[e.to]=u; pe[e.to]=j; }
            }
        }
        if(dist[t]==BIG) return 0;
        for(int i=0;i<N;++i) if(dist[i]!=BIG) h[i]+=dist[i];
        int a=need;
        for(int v=t;v!=s;v=pv[v]) a=min(a,g[pv[v]][pe[v]].cap);
        for(int v=t;v!=s;v=pv[v]) {
            Edge &e=g[pv[v]][pe[v]]; ans+=(long long)a*e.cost;
            e.cap-=a; g[v][e.rev].cap+=a;
        }
        need-=a;
    }
    cout<<ans<<'\n';
}
