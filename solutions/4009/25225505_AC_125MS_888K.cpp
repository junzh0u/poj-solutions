// POJ 4009 - Porcelain Exhibitions
// Model: gpt-6.1-sol
// Walk left faces using angularly sorted half-edges, then build their dual.
// Connectedness makes each face a single walk, including repeated bridges.
// First satisfy every minimum using max flow, then raise sink capacities
// to the maximum and augment the same residual network.
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
using namespace std;
typedef long long LL;
struct E { int v,r; LL c; E(int V,int R,LL C):v(V),r(R),c(C){} };
struct Flow {
 vector<vector<E> > a; vector<int> d,it;
 Flow(int n):a(n),d(n),it(n){}
 int add(int u,int v,LL c) { int k=a[u].size(); a[u].push_back(E(v,a[v].size(),c));a[v].push_back(E(u,k,0));return k; }
 bool bfs(int s,int t){fill(d.begin(),d.end(),-1);queue<int>q;q.push(s);d[s]=0;while(!q.empty()){int u=q.front();q.pop();for(int i=0;i<(int)a[u].size();++i)if(a[u][i].c&&d[a[u][i].v]<0){d[a[u][i].v]=d[u]+1;q.push(a[u][i].v);}}return d[t]>=0;}
 LL dfs(int u,int t,LL c){if(u==t)return c;for(int &i=it[u];i<(int)a[u].size();++i){E &e=a[u][i];if(e.c&&d[e.v]==d[u]+1){LL z=dfs(e.v,t,min(c,e.c));if(z){e.c-=z;a[e.v][e.r].c+=z;return z;}}}return 0;}
 LL run(int s,int t){LL ans=0,z;while(bfs(s,t)){fill(it.begin(),it.end(),0);while((z=dfs(s,t,((LL)1<<60))))ans+=z;}return ans;}
};
struct H {int u,v; double angle; H(int U,int V,double A):u(U),v(V),angle(A){} };
vector<H> h;
bool cmp(int a,int b){return h[a].angle<h[b].angle;}
int main(){ios::sync_with_stdio(false);cin.tie(0);int n,m;LL lo,hi,p;
while(cin>>n>>m>>lo>>hi>>p && n){vector<LL>x(n),y(n),w(m);for(int i=0;i<n;++i)cin>>x[i]>>y[i];h.clear();vector<vector<int> >adj(n);
for(int i=0;i<m;++i){int u,v;cin>>u>>v>>w[i];h.push_back(H(u,v,atan2((double)(y[v]-y[u]),(double)(x[v]-x[u]))));h.push_back(H(v,u,atan2((double)(y[u]-y[v]),(double)(x[u]-x[v]))));adj[u].push_back(2*i);adj[v].push_back(2*i+1);}
vector<int>pos(2*m),nxt(2*m),face(2*m,-1);for(int u=0;u<n;++u){sort(adj[u].begin(),adj[u].end(),cmp);for(int j=0;j<(int)adj[u].size();++j)pos[adj[u][j]]=j;}
for(int i=0;i<2*m;++i){int v=h[i].v,k=adj[v].size();nxt[i]=adj[v][(pos[i^1]+k-1)%k];}
vector<LL>area;for(int i=0;i<2*m;++i)if(face[i]<0){int j=i,id=area.size();LL a=0;do{face[j]=id;int u=h[j].u,v=h[j].v;a+=x[u]*y[v]-x[v]*y[u];j=nxt[j];}while(j!=i);area.push_back(a);}
int f=area.size(),s=f,t=f+1,cnt=0;Flow fl(f+2);vector<int>sink(f,-1);for(int i=0;i<f;++i)if(area[i]>0){++cnt;fl.add(s,i,min(area[i]*(p/2),hi*(LL)2000));sink[i]=fl.add(i,t,lo);}
for(int i=0;i<m;++i){int u=face[2*i],v=face[2*i+1];if(u!=v&&area[u]>0&&area[v]>0){fl.add(u,v,w[i]);fl.add(v,u,w[i]);}}
LL a=fl.run(s,t);if(a!=lo*cnt){cout<<-1<<'\n';continue;}for(int i=0;i<f;++i)if(sink[i]>=0)fl.a[i][sink[i]].c+=hi-lo;cout<<a+fl.run(s,t)<<'\n';
}return 0;}
