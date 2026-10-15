// POJ 3116 - MegaCheckers
// Model: gpt-6.1-sol
// Search all capture sequences, removing and restoring each jumped piece.
// A piece may revisit an emptied home; only opponents, not homes, are used once.
// A reachable-opponent upper bound prunes branches that cannot improve the best.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int to, id; Edge(int t,int i):to(t),id(i){} };
vector<Edge> adj[200];
bool used[100], seen[200], counted[100];
int best;
int bound(int s) {
    memset(seen,0,sizeof(seen)); memset(counted,0,sizeof(counted));
    int q[200], head=0,tail=0,total=0; q[tail++]=s;seen[s]=true;
    while(head<tail) {
        int v=q[head++];
        for(unsigned i=0;i<adj[v].size();++i) {
            Edge e=adj[v][i]; if(used[e.id]) continue;
            if(!counted[e.id]) { counted[e.id]=true; ++total; }
            if(!seen[e.to]) { seen[e.to]=true;q[tail++]=e.to; }
        }
    }
    return total;
}
void dfs(int v,int depth) {
    if(depth>best) best=depth;
    if(depth+bound(v)<=best) return;
    for(unsigned i=0;i<adj[v].size();++i) {
        Edge e=adj[v][i]; if(used[e.id]) continue;
        used[e.id]=true; dfs(e.to,depth+1); used[e.id]=false;
    }
}
int main() {
    int n,m,a[20][20],id[20][20];
    while(scanf("%d%d",&n,&m)==2 && (n||m)) {
        memset(a,-1,sizeof(a)); int homes=0;
        for(int r=0;r<n;++r) for(int c=0;c<m;++c) if((r+c)%2==0) {
            scanf("%d",&a[r][c]);id[r][c]=homes++;
        }
        best=0;
        for(int sr=0;sr<n;++sr) for(int sc=0;sc<m;++sc) if(a[sr][sc]==1) {
            for(int i=0;i<homes;++i) adj[i].clear();
            a[sr][sc]=0;int ei=0;
            for(int r=0;r<n;++r) for(int c=0;c<m;++c) if(a[r][c]==2) {
                for(int d=-1;d<=1;d+=2) {
                    int r1=r-1,c1=c-d,r2=r+1,c2=c+d;
                    if(r1>=0 && r2<n && c1>=0 && c1<m && c2>=0 && c2<m && a[r1][c1]==0 && a[r2][c2]==0) {
                        int x=id[r1][c1],y=id[r2][c2];
                        adj[x].push_back(Edge(y,ei));adj[y].push_back(Edge(x,ei));
                    }
                }
                ++ei;
            }
            memset(used,0,sizeof(used)); dfs(id[sr][sc],0);a[sr][sc]=1;
        }
        printf("%d\n",best);
    }
    return 0;
}
