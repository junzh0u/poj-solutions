// POJ 2113 - Agents
// Model: gpt-6-astra
// Find a dominating set of at most three vertices using closed-neighborhood
// bit masks. Enumerate its color partitions; all remaining vertices then
// have at most two colors, so each equal-color edge becomes a 2-SAT clause.
// The bad guys are not identified in the input: any dominating set suffices.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
const int N=500, W=16;
int n,m,b[3],col[N],opt[N][2];
bool adj[N][N];
unsigned mask[N][W],full[W];
vector<int> g[2*N];
int num[2*N],low[2*N],comp[2*N],stk[2*N],timer,top,ncomp;
bool inside[2*N];
void dfs(int u) {
    num[u]=low[u]=++timer; stk[top++]=u; inside[u]=true;
    for (size_t i=0;i<g[u].size();++i) {
        int v=g[u][i];
        if (!num[v]) { dfs(v); low[u]=min(low[u],low[v]); }
        else if (inside[v]) low[u]=min(low[u],num[v]);
    }
    if (low[u]==num[u]) {
        int v;
        do { v=stk[--top]; inside[v]=false; comp[v]=ncomp; } while(v!=u);
        ++ncomp;
    }
}
bool dominate() {
    int words=(n+31)/32;
    for(int i=0;i<n;++i) for(int j=i;j<n;++j) {
        unsigned missing[W];
        for(int w=0;w<words;++w) missing[w]=full[w]&~(mask[i][w]|mask[j][w]);
        for(int k=j;k<n;++k) {
            int w=0;
            while(w<words && !(missing[w]&~mask[k][w])) ++w;
            if(w==words) { b[0]=i; b[1]=j; b[2]=k; return true; }
        }
    }
    return false;
}
bool attempt(int c1,int c2) {
    int bc[3]={0,c1,c2};
    memset(col,-1,sizeof(col));
    for(int i=0;i<3;++i) {
        if(col[b[i]]!=-1 && col[b[i]]!=bc[i]) return false;
        col[b[i]]=bc[i];
    }
    for(int i=0;i<n;++i) {
        if(col[i]!=-1) { opt[i][0]=opt[i][1]=col[i]; continue; }
        bool banned[3]={false,false,false};
        for(int j=0;j<3;++j) if(adj[i][b[j]]) banned[bc[j]]=true;
        int cnt=0;
        for(int c=0;c<3;++c) if(!banned[c]) opt[i][cnt++]=c;
        if(!cnt) return false;
        if(cnt==1) opt[i][1]=opt[i][0];
    }
    for(int i=0;i<2*n;++i) g[i].clear();
    for(int i=0;i<n;++i) for(int j=i;j<n;++j) if(adj[i][j]) {
        for(int x=0;x<2;++x) for(int y=0;y<2;++y) if(opt[i][x]==opt[j][y]) {
            g[2*i+x].push_back(2*j+(y^1));
            g[2*j+y].push_back(2*i+(x^1));
        }
    }
    memset(num,0,sizeof(num)); memset(inside,0,sizeof(inside));
    timer=top=ncomp=0;
    for(int i=0;i<2*n;++i) if(!num[i]) dfs(i);
    for(int i=0;i<n;++i) {
        if(comp[2*i]==comp[2*i+1]) return false;
        col[i]=opt[i][comp[2*i]<comp[2*i+1]?0:1];
    }
    return true;
}
int main() {
    while(scanf("%d%d",&n,&m)==2 && n) {
        memset(adj,0,sizeof(adj)); memset(mask,0,sizeof(mask)); memset(full,0,sizeof(full));
        for(int i=0;i<n;++i) { mask[i][i/32]|=1U<<(i%32); full[i/32]|=1U<<(i%32); }
        for(int i=0;i<m;++i) {
            int u,v; scanf("%d%d",&u,&v); adj[u][v]=adj[v][u]=true;
            mask[u][v/32]|=1U<<(v%32); mask[v][u/32]|=1U<<(u%32);
        }
        bool ok=false;
        if(dominate()) for(int c1=0;c1<=1 && !ok;++c1)
            for(int c2=0;c2<=c1+1 && !ok;++c2) ok=attempt(c1,c2);
        if(!ok) puts("The agents cannot be split");
        else {
            int ren[3]={-1,-1,-1}, next=0;
            for(int i=0;i<n;++i) {
                if(ren[col[i]]==-1) ren[col[i]]=next++;
                printf("%s%d",i?" ":"",ren[col[i]]);
            }
            puts("");
        }
    }
    return 0;
}
