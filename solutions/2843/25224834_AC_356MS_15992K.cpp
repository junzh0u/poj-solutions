// POJ 2843 - Cutting Cake
// Model: gpt-6.1-sol
// Enumerate each newly removed cell using per-row successor disjoint sets.
// A second disjoint set joins its already visited left and upper neighbors
// from this cut, counting the four-connected pieces inside the rectangle.
// Connections through cells outside the cut do not join removed pieces.
#include <cstdio>
#include <vector>
using namespace std;
vector<int> nxt, parent, sz, stamp;
int root(vector<int>& p, int x) {
    int r=x;
    while(p[r]!=r) r=p[r];
    while(p[x]!=x) { int y=p[x]; p[x]=r; x=y; }
    return r;
}
bool join(int a,int b) {
    a=root(parent,a); b=root(parent,b);
    if(a==b) return false;
    if(sz[a]<sz[b]) {int t=a;a=b;b=t;}
    parent[b]=a; sz[a]+=sz[b]; return true;
}
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    int w=n+2, total=w*w;
    nxt.resize(total); parent.resize(total); sz.resize(total); stamp.assign(total,0);
    for(int i=0;i<total;++i) nxt[i]=i;
    for(int k=1;k<=m;++k) {
        int x1,y1,x2,y2; scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
        int count=0;
        for(int x=x1;x<=x2;++x) {
            int end=x*w+y2;
            for(int id=root(nxt,x*w+y1);id<=end;id=root(nxt,id)) {
                stamp[id]=k; parent[id]=id; sz[id]=1; ++count;
                if(stamp[id-1]==k && join(id,id-1)) --count;
                if(stamp[id-w]==k && join(id,id-w)) --count;
                nxt[id]=root(nxt,id+1);
            }
        }
        printf("%d\n",count);
    }
    return 0;
}
