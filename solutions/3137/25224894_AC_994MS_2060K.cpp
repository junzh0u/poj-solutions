// POJ 3137 - Enjoyable Commutation
// Model: gpt-6.1-sol
// Yen's algorithm enumerates simple paths by length, then numeric node order.
// Each deviation bans earlier root vertices and the used next edges of paths
// sharing that root. Reverse Dijkstra and greedy reconstruction give the
// lexicographically first shortest spur. Positive lengths prevent spur cycles.
#include <cstdio>
#include <vector>
#include <set>
#include <cstring>
using namespace std;
const int INF=1000000000;
int n,w[51][51],goal;
bool blocked[51],ban[51][51];
struct Path { int cost; vector<int> v; };
struct Less { bool operator()(const Path&a,const Path&b)const {
    if(a.cost!=b.cost)return a.cost<b.cost;
    return a.v<b.v;
}};
Path shortest(int s) {
    int d[51]; bool used[51]={false};
    for(int i=1;i<=n;i++)d[i]=INF;
    d[goal]=0;
    for(int it=0;it<n;it++) {
        int u=-1;
        for(int i=1;i<=n;i++)if(!blocked[i]&&!used[i]&&(u<0||d[i]<d[u]))u=i;
        if(u<0||d[u]==INF)break;
        used[u]=true;
        for(int v=1;v<=n;v++)if(!blocked[v]&&w[v][u]&&!ban[v][u]&&d[v]>d[u]+w[v][u])d[v]=d[u]+w[v][u];
    }
    Path p;p.cost=d[s];
    if(d[s]==INF)return p;
    p.v.push_back(s);
    while(s!=goal) {
        for(int v=1;v<=n;v++)if(!blocked[v]&&w[s][v]&&!ban[s][v]&&d[s]==w[s][v]+d[v]) {
            s=v;p.v.push_back(s);break;
        }
    }
    return p;
}
int main() {
    int m,k,start;
    while(scanf("%d%d%d%d%d",&n,&m,&k,&start,&goal)==5&&n) {
        memset(w,0,sizeof(w));
        for(int i=0;i<m;i++){int u,v,d;scanf("%d%d%d",&u,&v,&d);w[u][v]=d;}
        memset(blocked,0,sizeof(blocked));memset(ban,0,sizeof(ban));
        vector<Path> chosen;set<Path,Less> candidates;
        Path p=shortest(start);
        if(p.cost!=INF)chosen.push_back(p);
        while(!chosen.empty()&&(int)chosen.size()<k) {
            p=chosen.back();int rootcost=0;
            memset(blocked,0,sizeof(blocked));
            for(int j=0;j+1<(int)p.v.size();j++) {
                memset(ban,0,sizeof(ban));
                for(int q=0;q<(int)chosen.size();q++) {
                    if((int)chosen[q].v.size()<=j+1)continue;
                    int t=0;while(t<=j&&chosen[q].v[t]==p.v[t])t++;
                    if(t>j)ban[chosen[q].v[j]][chosen[q].v[j+1]]=true;
                }
                Path spur=shortest(p.v[j]);
                if(spur.cost!=INF) {
                    Path c;c.cost=rootcost+spur.cost;
                    c.v.assign(p.v.begin(),p.v.begin()+j);
                    c.v.insert(c.v.end(),spur.v.begin(),spur.v.end());candidates.insert(c);
                }
                blocked[p.v[j]]=true;rootcost+=w[p.v[j]][p.v[j+1]];
            }
            if(candidates.empty())break;
            chosen.push_back(*candidates.begin());candidates.erase(candidates.begin());
        }
        if((int)chosen.size()<k)puts("None");
        else {p=chosen[k-1];for(int i=0;i<(int)p.v.size();i++)printf(i?"-%d":"%d",p.v[i]);puts("");}
    }
    return 0;
}
