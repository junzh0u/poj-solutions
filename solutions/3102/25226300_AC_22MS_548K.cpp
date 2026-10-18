// POJ 3102 - Bee Garden
// Model: gpt-6-astra
// Adding an edge creates one cycle. Traverse its edges once and every
// remaining tree edge twice, saving treeDistance(u,v)-distance(u,v).
// Omitting a cycle edge cannot improve this: triangle inequality bounds
// each edge by the sum of the others. Maximize this saving over legal roads.
// Existing vertices may not lie strictly inside the new segment; crossings
// of roads are allowed and do not create junctions. O(n^3) time, O(n^2) space.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
int x[201], y[201];
vector<int> adj[201];
double distTree[201][201];
double length(int a,int b) {
    double dx=x[a]-x[b],dy=y[a]-y[b];
    return sqrt(dx*dx+dy*dy);
}
void dfs(int root,int u,int parent) {
    for (size_t i=0;i<adj[u].size();++i) {
        int v=adj[u][i];
        if(v==parent) continue;
        distTree[root][v]=distTree[root][u]+length(u,v);
        dfs(root,v,u);
    }
}
bool inside(int a,int b,int c) {
    int dx=x[b]-x[a],dy=y[b]-y[a];
    int ex=x[c]-x[a],ey=y[c]-y[a];
    // Each product and their difference fit signed 32 bits (<= 800000000).
    if(dx*ey-dy*ex!=0) return false;
    return (x[c]-x[a])*(x[c]-x[b])+(y[c]-y[a])*(y[c]-y[b])<0;
}
int main() {
    int n;
    if(scanf("%d",&n)!=1) return 0;
    for(int i=1;i<=n;++i) scanf("%d%d",&x[i],&y[i]);
    for(int i=0;i<n;++i) {
        int a,b; scanf("%d%d",&a,&b);
        adj[a].push_back(b); adj[b].push_back(a);
    }
    for(int i=0;i<=n;++i) dfs(i,i,-1);
    double best=1e-8;
    int a=-1,b=-1;
    for(int i=0;i<=n;++i) for(int j=i+1;j<=n;++j) {
        double save=distTree[i][j]-length(i,j);
        if(save<=best) continue;
        bool legal=true;
        for(int k=0;k<=n;++k) if(k!=i && k!=j && inside(i,j,k)) {
            legal=false; break;
        }
        if(legal) { best=save; a=i; b=j; }
    }
    if(a<0) puts("-1"); else printf("%d %d\n",a,b);
    return 0;
}
