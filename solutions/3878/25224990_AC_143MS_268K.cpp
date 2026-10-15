// POJ 3878 - Haunted Graveyard
// Model: gpt-6.1-sol
// Build directed moves, making holes compulsory and the exit terminal.
// Bellman-Ford detects every negative cycle reachable from the entrance,
// even when that cycle cannot lead to the exit. Unreachable cycles do not count.
// Synchronous rounds bound finite distances by 900 edges * 10000 seconds.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int u,v,w; Edge(int a,int b,int c):u(a),v(b),w(c){} };
int main() {
    int W,H;
    while (scanf("%d%d",&W,&H)==2 && (W || H)) {
        int n=W*H,g,e,x,y,a,b,t;
        vector<int> kind(n,0);
        vector<Edge> edges;
        scanf("%d",&g);
        while(g--) { scanf("%d%d",&x,&y); kind[y*W+x]=1; }
        scanf("%d",&e);
        while(e--) {
            scanf("%d%d%d%d%d",&x,&y,&a,&b,&t);
            kind[y*W+x]=2;
            edges.push_back(Edge(y*W+x,b*W+a,t));
        }
        int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
        for(y=0;y<H;++y) for(x=0;x<W;++x) {
            int u=y*W+x;
            if(kind[u] || u==n-1) continue;
            for(int k=0;k<4;++k) {
                a=x+dx[k]; b=y+dy[k];
                if(a>=0 && a<W && b>=0 && b<H && kind[b*W+a]!=1)
                    edges.push_back(Edge(u,b*W+a,1));
            }
        }
        const int INF=1000000000;
        vector<int> d(n,INF); d[0]=0;
        bool changed=false;
        for(int round=0;round<n;++round) {
            vector<int> next=d; changed=false;
            for(unsigned k=0;k<edges.size();++k) {
                Edge z=edges[k];
                if(d[z.u]!=INF && d[z.u]+z.w<next[z.v]) {
                    next[z.v]=d[z.u]+z.w; changed=true;
                }
            }
            d.swap(next);
            if(!changed) break;
        }
        if(changed) puts("Never");
        else if(d[n-1]==INF) puts("Impossible");
        else printf("%d\n",d[n-1]);
    }
    return 0;
}
