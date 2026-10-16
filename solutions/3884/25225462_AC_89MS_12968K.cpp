// POJ 3884 - Gearbox
// Model: gpt-6.1-sol
// Propagate symbolic angular velocities through the rod graph. Each gear
// type is an independent positive variable; record its integer exponent.
// Every interlocking reverses direction and multiplies by teeth[a]/teeth[b].
// All rods can turn for every tooth assignment exactly when every edge
// agrees with both the parity and all exponents of the spanning forest.
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
struct Edge { int to, a, b; Edge(int v,int x,int y):to(v),a(x),b(y){} };
int main() {
    int cases; if(scanf("%d",&cases)!=1) return 0;
    while(cases--) {
        int ng,nr,ni; scanf("%d%d%d",&ng,&nr,&ni);
        vector<int> type(ng),rod(ng);
        for(int i=0;i<ng;++i) scanf("%d%d",&type[i],&rod[i]);
        vector< vector<Edge> > adj(nr);
        for(int i=0;i<ni;++i) {
            int a,b; scanf("%d%d",&a,&b);
            adj[rod[a]].push_back(Edge(rod[b],type[a],type[b]));
            adj[rod[b]].push_back(Edge(rod[a],type[b],type[a]));
        }
        vector<int> power(nr*100,0),parity(nr,-1),queue(nr);
        bool ok=true;
        for(int root=0;root<nr && ok;++root) if(parity[root]<0) {
            int head=0,tail=0; queue[tail++]=root; parity[root]=0;
            while(head<tail && ok) {
                int u=queue[head++];
                for(unsigned j=0;j<adj[u].size() && ok;++j) {
                    const Edge &e=adj[u][j]; int v=e.to;
                    int *pu=&power[u*100], *pv=&power[v*100];
                    if(parity[v]<0) {
                        parity[v]=parity[u]^1;
                        memcpy(pv,pu,100*sizeof(int));
                        ++pv[e.a]; --pv[e.b]; queue[tail++]=v;
                    } else {
                        if(parity[v]!=(parity[u]^1)) {ok=false;break;}
                        for(int t=1;t<100;++t)
                            if(pv[t]!=pu[t]+(t==e.a)-(t==e.b)) {ok=false;break;}
                    }
                }
            }
        }
        puts(ok?"ok":"jammed");
    }
    return 0;
}
