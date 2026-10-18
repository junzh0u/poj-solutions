// POJ 2343 - Airline company
// Model: gpt-6-astra
// Search permutations, first using local repairs and then exhaustive backtracking.
// Only vertices of degree at least two constrain the incident labels' gcd.
// Degree-two chains give a necessary parity bound; disconnected graphs matter.
// Every YES is checked; NO follows a parity obstruction or exhaustive search.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
const int M=1226;
int n,m,a[M],b[M],label[M],deg[51],val[51],leftv[51],ord[M];
vector<int> inc[51],adj[M];
bool used[M],seen[M];
int gcd(int x,int y){while(y){int z=x%y;x=y;y=z;}return x;}
bool bad(int v){
    if(deg[v]<2)return false;
    int g=0;
    for(unsigned i=0;i<inc[v].size();++i)g=gcd(g,label[inc[v][i]]);
    return g!=1;
}
bool valid(){for(int v=1;v<=n;++v)if(bad(v))return false;return true;}
bool cmp(int x,int y){
    int dx=min(deg[a[x]],deg[b[x]]),dy=min(deg[a[y]],deg[b[y]]);
    if(dx!=dy)return dx<dy;
    return x<y;
}
bool search(int depth){
    if(depth==m)return true;
    int e=ord[depth],u=a[e],v=b[e];
    int gu=val[u],gv=val[v];
    --leftv[u];--leftv[v];
    for(int k=1;k<=m;++k)if(!used[k]){
        int nu=gcd(gu,k),nv=gcd(gv,k);
        if((deg[u]>1 && leftv[u]==0 && nu!=1) ||
           (deg[v]>1 && leftv[v]==0 && nv!=1))continue;
        label[e]=k;used[k]=true;val[u]=nu;val[v]=nv;
        if(search(depth+1))return true;
        used[k]=false;
    }
    val[u]=gu;val[v]=gv;++leftv[u];++leftv[v];
    return false;
}
int main(){
    if(scanf("%d%d",&n,&m)!=2)return 0;
    for(int e=0;e<m;++e){
        scanf("%d%d",&a[e],&b[e]);
        inc[a[e]].push_back(e);inc[b[e]].push_back(e);
        ++deg[a[e]];++deg[b[e]];label[e]=e+1;ord[e]=e;
    }
    for(int v=1;v<=n;++v)if(deg[v]==2){
        int x=inc[v][0],y=inc[v][1];adj[x].push_back(y);adj[y].push_back(x);
    }
    int maxeven=0;
    for(int e=0;e<m;++e)if(!seen[e]){
        vector<int> q;q.push_back(e);seen[e]=true;
        bool cycle=true;
        for(unsigned i=0;i<q.size();++i){
            int x=q[i];if(adj[x].size()!=2)cycle=false;
            for(unsigned j=0;j<adj[x].size();++j){
                int y=adj[x][j];if(!seen[y]){seen[y]=true;q.push_back(y);}
            }
        }
        maxeven+=(int(q.size())+(cycle?0:1))/2;
    }
    if(m/2>maxeven){puts("NO");return 0;}
    bool ok=valid();
    for(int step=0;!ok && step<50000;++step){
        int bads[51],nb=0;
        for(int v=1;v<=n;++v)if(bad(v))bads[nb++]=v;
        if(nb==0){ok=true;break;}
        int v=bads[rand()%nb],x=inc[v][rand()%deg[v]],y=rand()%m;
        int vs[4]={a[x],b[x],a[y],b[y]};
        sort(vs,vs+4);int count=int(unique(vs,vs+4)-vs),before=0,after=0;
        for(int i=0;i<count;++i)before+=bad(vs[i]);
        swap(label[x],label[y]);
        for(int i=0;i<count;++i)after+=bad(vs[i]);
        if(after>before && rand()%100!=0)swap(label[x],label[y]);
        if(step%1000==999)random_shuffle(label,label+m);
        ok=valid();
    }
    if(!ok){
        sort(ord,ord+m,cmp);
        for(int v=1;v<=n;++v)leftv[v]=deg[v];
        ok=search(0);
    }
    if(!ok){puts("NO");return 0;}
    puts("YES");
    for(int e=0;e<m;++e)printf("%d%c",label[e],e+1==m?'\n':' ');
    if(m==0)puts("");
    return 0;
}
