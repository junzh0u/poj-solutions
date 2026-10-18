// POJ 3538 - Domestic Networks
// Model: gpt-6-astra
// Kruskal's sorted edge lengths are componentwise no larger than those
// of any spanning tree. Transferring cable categories to these edges
// cannot increase either inventory use or cost, so an MST suffices.
// Subset-sum DP chooses category 5 edges; immutable first-discovery
// predecessors reconstruct a valid assignment. Zero-length edges use 6.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int a,b,w,id; };
bool lessEdge(const Edge &a,const Edge &b) {
    if(a.w!=b.w) return a.w<b.w;
    return a.id<b.id;
}
int parent[1001];
int root(int a) { return parent[a]==a?a:parent[a]=root(parent[a]); }
int main() {
    int n,m;
    if(!(cin>>n>>m)) return 0;
    vector<Edge> edges(m),tree;
    for(int i=0;i<m;++i) { cin>>edges[i].a>>edges[i].b>>edges[i].w; edges[i].id=i+1; }
    int p5,q5,p6,q6; cin>>p5>>q5>>p6>>q6;
    for(int i=1;i<=n;++i) parent[i]=i;
    sort(edges.begin(),edges.end(),lessEdge);
    int total=0;
    for(int i=0;i<m;++i) {
        int a=root(edges[i].a),b=root(edges[i].b);
        if(a!=b) { parent[a]=b; tree.push_back(edges[i]); total+=edges[i].w; }
    }
    if((int)tree.size()!=n-1 || total>q5+q6) { cout<<"Impossible\n"; return 0; }
    vector<int> last(q5+1,-1);
    last[0]=-2;
    for(int i=0;i<n-1;++i) {
        int w=tree[i].w;
        for(int j=q5;j>=w;--j)
            if(last[j]==-1 && last[j-w]!=-1) last[j]=i;
    }
    int best=-1,cost=2000000000;
    for(int j=0;j<=q5;++j) if(last[j]!=-1 && total-j<=q6) {
        int c=j*p5+(total-j)*p6;
        if(c<cost) { cost=c; best=j; }
    }
    if(best<0) { cout<<"Impossible\n"; return 0; }
    vector<int> category(n-1,6);
    while(best>0) { int i=last[best]; category[i]=5; best-=tree[i].w; }
    cout<<cost<<'\n';
    for(int i=0;i<n-1;++i) cout<<tree[i].id<<' '<<category[i]<<'\n';
    return 0;
}
