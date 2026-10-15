// POJ 3815 - The Ninja Way
// Model: gpt-6.1-sol
// Integer positions form difference constraints: adjacent input positions
// differ by at least one, and consecutive heights are at most D apart.
// Unit spacing proves feasibility exactly when every required index span
// is at most D. Shortest-path potentials attain the maximum endpoint span.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>
using namespace std;
struct Edge { int u,v,w; Edge(int a,int b,int c):u(a),v(b),w(c){} };
int main() {
    int n,d;
    while(scanf("%d%d",&n,&d)==2 && n) {
        vector<pair<int,int> > h;
        for(int i=0;i<n;++i) { int x; scanf("%d",&x); h.push_back(make_pair(x,i)); }
        sort(h.begin(),h.end());
        vector<Edge> e;
        for(int i=1;i<n;++i) e.push_back(Edge(i,i-1,-1));
        bool feasible=true;
        for(int i=1;i<n;++i) {
            int a=h[i-1].second,b=h[i].second;
            if(abs(a-b)>d) feasible=false;
            e.push_back(Edge(a,b,d)); e.push_back(Edge(b,a,d));
        }
        if(!feasible) { puts("-1"); continue; }
        int s=min(h[0].second,h[n-1].second),t=max(h[0].second,h[n-1].second);
        const int INF=2000000000;
        vector<int> dist(n,INF); dist[s]=0;
        for(int it=1;it<n;++it) {
            bool changed=false;
            for(size_t j=0;j<e.size();++j) {
                Edge a=e[j];
                if(dist[a.u]!=INF && dist[a.v]>dist[a.u]+a.w) {
                    dist[a.v]=dist[a.u]+a.w; changed=true;
                }
            }
            if(!changed) break;
        }
        printf("%d\n",dist[t]);
    }
    return 0;
}
