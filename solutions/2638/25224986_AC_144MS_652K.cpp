// POJ 2638 - Kingdom
// Model: gpt-6.1-sol
// Floyd-Warshall gives distances to both cities. Each road receives its
// minimum possible worst distance to a city, measured in half-time units.
// Binary search these thresholds; a minimum cut counts the soldiers needed.
// Endpoints represent the limiting placement arbitrarily close to a town.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
const int INF=10000000;
struct Road { int u,v,w,c; };
int n,g,e,d[155][155],cap[155][155],lev[155],it[155];
vector<Road> roads;
int dfs(int u,int f) {
 if(u==n-1) return f;
 for(int &v=it[u];v<n;++v) if(cap[u][v] && lev[v]==lev[u]+1) {
  int z=dfs(v,min(f,cap[u][v]));
  if(z) {cap[u][v]-=z;cap[v][u]+=z;return z;}
 }
 return 0;
}
bool feasible(int t) {
 for(int i=0;i<n;++i)for(int j=0;j<n;++j)cap[i][j]=0;
 for(int i=0;i<e;++i) {Road r=roads[i];cap[r.u][r.v]=cap[r.v][r.u]=(r.c<=t?1:g+1);}
 int flow=0;
 while(flow<=g) {
  fill(lev,lev+n,-1);queue<int> q;q.push(n-2);lev[n-2]=0;
  while(!q.empty()){int u=q.front();q.pop();for(int v=0;v<n;++v)if(cap[u][v] && lev[v]<0){lev[v]=lev[u]+1;q.push(v);}}
  if(lev[n-1]<0)return true;
  fill(it,it+n,0);int z;
  while(flow<=g && (z=dfs(n-2,g+1-flow))>0)flow+=z;
 }
 return false;
}
int zip(int x){return x==95050?n-2:x==104729?n-1:x;}
int main(){int towns;
 while(scanf("%d",&towns)==1 && towns){scanf("%d%d",&g,&e);n=towns+2;roads.clear();
  for(int i=0;i<n;++i)for(int j=0;j<n;++j)d[i][j]=(i==j?0:INF);
  for(int i=0;i<e;++i){int a,b,w;scanf("%d%d%d",&a,&b,&w);Road r;r.u=zip(a);r.v=zip(b);r.w=w;r.c=0;roads.push_back(r);d[r.u][r.v]=d[r.v][r.u]=w;}
  for(int k=0;k<n;++k)for(int i=0;i<n;++i)for(int j=0;j<n;++j)d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
  vector<int> vals;vals.push_back(0);
  for(int i=0;i<e;++i){Road &r=roads[i];int u=r.u,v=r.v,w=r.w,s=n-2,t=n-1;
   int x[6]={0,2*w,w+d[s][v]-d[s][u],w+d[t][v]-d[t][u],w+d[t][v]-d[s][u],w+d[s][v]-d[t][u]};
   r.c=INF;
   for(int j=0;j<6;++j){int p=max(0,min(2*w,x[j]));int a=min(2*d[s][u]+p,2*d[s][v]+2*w-p);int b=min(2*d[t][u]+p,2*d[t][v]+2*w-p);r.c=min(r.c,max(a,b));}
   vals.push_back(r.c);
  }
  sort(vals.begin(),vals.end());vals.erase(unique(vals.begin(),vals.end()),vals.end());
  if(!feasible(vals.back()))puts("Impossible");else{int l=0,h=(int)vals.size()-1;while(l<h){int m=(l+h)/2;if(feasible(vals[m]))h=m;else l=m+1;}printf("%.1f\n",vals[l]/2.0);}
 }
 return 0;
}
