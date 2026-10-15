// POJ 1842 - Parking
// Model: gpt-6.1-sol
// Process blocking cars from the bottom of the exit path upwards.
// Each consumes the shallowest unused vacancy in its descendant subtree.
// Shifting cars toward that vacancy costs its depth minus the blocker's depth.
// Path rooms cannot be final destinations; Ralph's car and subtree are blocked.
#include <cstdio>
#include <vector>
using namespace std;
const int M=5005;
vector<int> child[M];
int par[M],dep[M],tin[M],tout[M],timer;
bool path[M],occupied[M];
void visit(int u) {
    tin[u]=++timer;
    for (int i=0;i<(int)child[u].size();++i) {
        int v=child[u][i]; dep[v]=dep[u]+1; visit(v);
    }
    tout[u]=timer;
}
bool below(int v,int u) {return tin[u]<=tin[v] && tin[v]<=tout[u];}
int main() {
    int n,p,k;
    if(scanf("%d%d%d",&n,&p,&k)!=3) return 0;
    for(int u=1;u<=n;++u) {
        int t;scanf("%d",&t);
        while(t--) {int v;scanf("%d",&v);child[u].push_back(v);par[v]=u;}
    }
    for(int i=0;i<k;++i) {int v;scanf("%d",&v);occupied[v]=true;}
    visit(1);
    for(int v=p;v;v=par[v]) path[v]=true;
    int answer=0;
    for(int u=par[p];u!=1;u=par[u]) if(occupied[u]) {
        int best=0;
        for(int v=2;v<=n;++v)
            if(!path[v] && !occupied[v] && below(v,u) && !below(v,p)
               && (!best || dep[v]<dep[best])) best=v;
        if(!best) {puts("not solvable");return 0;}
        answer+=dep[best]-dep[u]; occupied[best]=true;
    }
    printf("%d\n",answer);
    return 0;
}
