// POJ 3847 - Moving to Nuremberg
// Model: gpt-6.1-sol
// Sum weighted distances at one root, then reroot across each edge.
// Moving into a subtree shortens its trips and lengthens all other trips.
// Trips are round trips; with no visits every station is optimal.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct FastInput {
    enum { SIZE = 65536 };
    char buf[SIZE]; int pos, len;
    FastInput(): pos(0), len(0) {}
    int get() { if(pos == len) { len = fread(buf,1,SIZE,stdin); pos=0; if(!len) return EOF; } return buf[pos++]; }
    int next() { int c=get(), x=0; while(c<=32 && c!=EOF) c=get(); while(c>32 && c!=EOF) { x=x*10+c-'0'; c=get(); } return x; }
};
struct Edge { int to, w, next; };
int main() {
    FastInput in; int cases=in.next();
    while(cases--) {
        int n=in.next();
        vector<int> head(n,-1), parent(n,-1), order, pw(n,0);
        vector<Edge> edges; edges.reserve(2*(n-1));
        for(int i=1;i<n;++i) {
            int a=in.next()-1,b=in.next()-1,w=in.next();
            Edge e={b,w,head[a]}; head[a]=edges.size(); edges.push_back(e);
            e.to=a; e.next=head[b]; head[b]=edges.size(); edges.push_back(e);
        }
        vector<long long> weight(n,0), sub(n,0), cost(n,0), dist(n,0);
        int m=in.next(); long long total=0;
        for(int i=0;i<m;++i) { int a=in.next()-1,f=in.next(); weight[a]=f; total+=f; }
        order.reserve(n); order.push_back(0); parent[0]=0;
        for(int i=0;i<n;++i) {
            int u=order[i];
            for(int e=head[u];e!=-1;e=edges[e].next) {
                int v=edges[e].to; if(v==parent[u]) continue;
                parent[v]=u; pw[v]=edges[e].w; dist[v]=dist[u]+pw[v]; order.push_back(v);
            }
        }
        for(int i=0;i<n;++i) { sub[i]=weight[i]; cost[0]+=weight[i]*dist[i]; }
        for(int i=n-1;i>0;--i) { int v=order[i]; sub[parent[v]]+=sub[v]; }
        long long best=cost[0];
        for(int i=1;i<n;++i) { int v=order[i]; cost[v]=cost[parent[v]]+(total-2*sub[v])*pw[v]; best=min(best,cost[v]); }
        printf("%lld\n",2*best);
        bool first=true;
        for(int i=0;i<n;++i) if(cost[i]==best) { if(!first) putchar(' '); printf("%d",i+1); first=false; }
        putchar('\n');
    }
    return 0;
}
