// POJ 3745 - Keep Contact
// Model: gpt-6-astra
// Construct two edge-disjoint rooted spanning trees with SPAN2.
// Keep the second tree and replace the first after removing a deepest
// first-tree edge leaving the second tree's vertex set. Then extend the
// second tree. The deepest-boundary rule is essential (see discussion).
// Each scheme has exactly n-1 assistants, the minimum possible.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int v, id; Edge(int a,int b):v(a),id(b){} };
int n,m;
vector<Edge> g[2005];
int p1[2005],p2[2005],dep[2005],q[2005];
vector<unsigned char> used;
bool first(int ban) {
    fill(p1,p1+n,-1); p1[0]=0; dep[0]=0;
    int h=0,t=0; q[t++]=0;
    while(h<t) {
        int u=q[h++];
        for(size_t j=0;j<g[u].size();++j) {
            Edge e=g[u][j];
            if(e.id==ban || used[e.id] || p1[e.v]!=-1) continue;
            p1[e.v]=e.id; dep[e.v]=dep[u]+1; q[t++]=e.v;
        }
    }
    return t==n;
}
int extend() {
    fill(used.begin(),used.end(),0);
    for(int v=1;v<n;++v) used[p1[v]]=1;
    int h=0,t=0;
    for(int v=0;v<n;++v) if(p2[v]!=-1) q[t++]=v;
    while(h<t) {
        int u=q[h++];
        for(size_t j=0;j<g[u].size();++j) {
            Edge e=g[u][j];
            if(used[e.id] || p2[e.v]!=-1) continue;
            p2[e.v]=e.id; q[t++]=e.v;
        }
    }
    return t;
}
int main() {
    while(scanf("%d%d",&n,&m)==2 && n) {
        for(int u=0,id=0;u<n;++u) {
            g[u].clear(); int k; scanf("%d",&k);
            while(k--) { int v;scanf("%d",&v);g[u].push_back(Edge(v-1,++id)); }
        }
        used.assign(m+1,0);
        bool ok=first(-1);
        fill(p2,p2+n,-1);p2[0]=0;
        while(ok && extend()<n) {
            int w=-1;
            for(int u=0;u<n;++u) if(p2[u]!=-1)
                for(size_t j=0;j<g[u].size();++j) {
                    Edge e=g[u][j];
                    if(p2[e.v]==-1 && p1[e.v]==e.id && (w==-1 || dep[e.v]>dep[w])) w=e.v;
                }
            if(w==-1) {ok=false;break;}
            int ban=p1[w];
            fill(used.begin(),used.end(),0);
            for(int v=1;v<n;++v) if(p2[v]!=-1) used[p2[v]]=1;
            ok=first(ban);
        }
        if(!ok) puts("NONE\n");
        else {
            puts("YES");
            for(int c=0;c<2;++c) {
                vector<int> a;for(int v=1;v<n;++v) a.push_back(c?p2[v]:p1[v]);
                sort(a.begin(),a.end());
                for(size_t i=0;i<a.size();++i) printf("%s%d",i?" ":"",a[i]);
                puts("");
            }
            puts("");
        }
    }
}
