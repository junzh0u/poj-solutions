// POJ 3826 - Trip the Lights Fantastic
// Model: gpt-6.1-sol
// Dijkstra labels the time ready to leave each intermediate light.
// Red arrivals wait for the next green and incur a five-second restart;
// the destination uses arrival time, without waiting or restarting.
// Green includes its first instant; red includes its first instant.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int v, t; Edge(int a,int b):v(a),t(b){} };
int main() {
    int n,m,s,e;
    while(scanf("%d%d%d%d",&n,&m,&s,&e)==4 && n) {
        int go[100],cycle[100],d[100]; bool used[100]={false};
        vector<Edge> adj[100];
        for(int i=0;i<n;++i) { int g,y,r; scanf("%d%d%d",&g,&y,&r); go[i]=g+y; cycle[i]=g+y+r; d[i]=100000000; }
        for(int i=0;i<m;++i) { int a,b,t; scanf("%d%d%d",&a,&b,&t); adj[a].push_back(Edge(b,t)); adj[b].push_back(Edge(a,t)); }
        d[s]=5;
        for(int step=0;step<n;++step) {
            int u=-1;
            for(int i=0;i<n;++i) if(!used[i] && (u<0 || d[i]<d[u])) u=i;
            if(u==e || u<0) break;
            used[u]=true;
            for(unsigned int j=0;j<adj[u].size();++j) {
                int v=adj[u][j].v, t=d[u]+adj[u][j].t;
                if(used[v]) continue;
                int phase=t%cycle[v];
                if(v!=e && phase>=go[v]) t+=cycle[v]-phase+5;
                d[v]=min(d[v],t);
            }
        }
        printf("%d:%02d\n",d[e]/60,d[e]%60);
    }
    return 0;
}
