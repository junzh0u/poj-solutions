// POJ 3759 - Simple Distributed computing system
// Model: gpt-6-astra
// Maximum flow assigns demand to compatible servers. Consolidate each
// application's positive loads among nonfull servers, filling a receiver
// or emptying a donor each time. Full servers never become nonfull, so
// processing later applications preserves earlier efficiency guarantees.
// Output is a witness: it need not coincide with the sample placement.
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Edge { int to, rev, cap; Edge(int t,int r,int c):to(t),rev(r),cap(c){} };
vector<Edge> g[405];
int level[405], it[405];
int add(int a,int b,int c) { int k=g[a].size(); g[a].push_back(Edge(b,g[b].size(),c)); g[b].push_back(Edge(a,k,0)); return k; }
bool bfs(int s,int t) {
    fill(level,level+405,-1); queue<int> q; q.push(s); level[s]=0;
    while(!q.empty()) { int v=q.front();q.pop(); for(size_t i=0;i<g[v].size();++i) { Edge &e=g[v][i]; if(e.cap && level[e.to]<0) { level[e.to]=level[v]+1;q.push(e.to); } } }
    return level[t]>=0;
}
int dfs(int v,int t,int f) {
    if(v==t)return f;
    for(int &i=it[v];i<(int)g[v].size();++i) { Edge &e=g[v][i]; if(e.cap && level[e.to]==level[v]+1) { int z=dfs(e.to,t,min(f,e.cap)); if(z) {e.cap-=z;g[e.to][e.rev].cap+=z;return z;} } }
    return 0;
}
int load[200][200], rem[200], idx[200][200];
vector<int> apps[200];
int main() {
    int n,m; if(scanf("%d%d",&n,&m)!=2)return 0;
    int s=n+m,t=s+1;
    for(int a=0;a<n;++a) {int d;scanf("%d",&d);add(m+a,t,d);}
    for(int j=0;j<m;++j) {int k;scanf("%d%d",&rem[j],&k);add(s,j,rem[j]);for(int x=0;x<k;++x) {int a;scanf("%d",&a);apps[j].push_back(a);idx[j][a]=add(j,m+a,200000);}}
    int total=0;
    while(bfs(s,t)) {fill(it,it+405,0);int z;while((z=dfs(s,t,200000))!=0)total+=z;}
    for(int j=0;j<m;++j)for(size_t k=0;k<apps[j].size();++k) {int a=apps[j][k];Edge &e=g[j][idx[j][a]];load[j][a]=g[e.to][e.rev].cap;rem[j]-=load[j][a];}
    for(int a=0;a<n;++a) {
        int receiver=-1;
        for(int j=0;j<m;++j)if(rem[j]>0 && load[j][a]>0) {
            if(receiver<0) {receiver=j;continue;}
            int z=min(rem[receiver],load[j][a]);
            load[receiver][a]+=z;rem[receiver]-=z;load[j][a]-=z;rem[j]+=z;
            if(rem[receiver]==0)receiver=load[j][a]>0?j:-1;
        }
    }
    printf("%d\n",total);
    for(int j=0;j<m;++j) {for(size_t k=0;k<apps[j].size();++k)printf("%s%d",k?" ":"",load[j][apps[j][k]]);puts("");}
    return 0;
}
