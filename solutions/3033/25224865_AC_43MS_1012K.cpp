// POJ 3033 - Traveling Salesman
// Model: gpt-6.1-sol
// Match unordered polygon edges by their exact integer endpoints, then BFS
// the country adjacency graph. Sharing only a vertex does not allow crossing.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Point {
    int x,y,z;
    bool operator<(const Point& p) const {
        if(x!=p.x) return x<p.x;
        if(y!=p.y) return y<p.y;
        return z<p.z;
    }
    bool operator==(const Point& p) const {
        return x==p.x && y==p.y && z==p.z;
    }
};
struct Edge {
    Point a,b; int country;
    bool operator<(const Edge& e) const {
        if(a<e.a) return true;
        if(e.a<a) return false;
        return b<e.b;
    }
};
int main() {
    int c;
    while(scanf("%d",&c)==1 && c) {
        vector<Edge> edges;
        edges.reserve(c*20);
        for(int i=0;i<c;++i) {
            int n; scanf("%d",&n);
            Point p[20];
            for(int j=0;j<n;++j) scanf("%d%d%d",&p[j].x,&p[j].y,&p[j].z);
            for(int j=0;j<n;++j) {
                Edge e; e.a=p[j]; e.b=p[(j+1)%n]; e.country=i;
                if(e.b<e.a) swap(e.a,e.b);
                edges.push_back(e);
            }
        }
        sort(edges.begin(),edges.end());
        vector<vector<int> > adj(c);
        for(size_t i=1;i<edges.size();++i) {
            if(edges[i].a==edges[i-1].a && edges[i].b==edges[i-1].b) {
                int u=edges[i].country,v=edges[i-1].country;
                adj[u].push_back(v); adj[v].push_back(u);
            }
        }
        int m; scanf("%d",&m);
        vector<int> dist(c),q(c);
        while(m--) {
            int s,t; scanf("%d%d",&s,&t); --s; --t;
            fill(dist.begin(),dist.end(),-1);
            int head=0,tail=0; q[tail++]=s; dist[s]=0;
            while(head<tail && dist[t]<0) {
                int u=q[head++];
                for(size_t j=0;j<adj[u].size();++j) {
                    int v=adj[u][j];
                    if(dist[v]<0) { dist[v]=dist[u]+1; q[tail++]=v; }
                }
            }
            printf("%d\n",dist[t]);
        }
    }
    return 0;
}
