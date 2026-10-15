// POJ 2822 - TN's Kingdom IV - Collapse
// Model: gpt-6.1-sol
// Add an exterior west-to-east edge to split the outer face. A minimum
// undirected cut is a shortest path between its two sides in the planar dual.
// Enumerate faces by angular half-edge order; exclude the exterior edge from
// the dual. Only the source component matters; disconnected terminals give 0.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>
#include <iostream>
#include <iomanip>
using namespace std;
struct Edge { int u,v; double c; Edge(int a,int b,double d):u(a),v(b),c(d){} };
struct Half { int to; double angle; Half(int t,double a):to(t),angle(a){} };
vector<Half> h;
struct Compare { bool operator()(int a,int b)const{return h[a].angle<h[b].angle;} };
vector<int> par;
int root(int a){while(par[a]!=a){par[a]=par[par[a]];a=par[a];}return a;}
int main(){
 int n,m;if(scanf("%d%d",&n,&m)!=2)return 0;
 vector<double>x(n),y(n);par.resize(n);int s=0,t=0;
 for(int i=0;i<n;++i){scanf("%lf%lf",&x[i],&y[i]);par[i]=i;if(x[i]<x[s])s=i;if(x[i]>x[t])t=i;}
 vector<Edge> e;e.reserve(m);
 for(int i=0;i<m;++i){int a,b;double c;scanf("%d%d%lf",&a,&b,&c);e.push_back(Edge(a,b,c));par[root(a)]=root(b);}
 if(root(s)!=root(t)){cout<<"0.0000\n";return 0;}
 vector<vector<int> >adj(n);vector<double>cap;
 for(int i=0;i<m;++i)if(root(e[i].u)==root(s)&&e[i].u!=e[i].v){
  int a=e[i].u,b=e[i].v,k=h.size();double z=atan2(y[b]-y[a],x[b]-x[a]);
  h.push_back(Half(b,z));h.push_back(Half(a,atan2(y[a]-y[b],x[a]-x[b])));
  adj[a].push_back(k);adj[b].push_back(k+1);cap.push_back(e[i].c);
 }
 int artificial=h.size();h.push_back(Half(t,acos(-1.0)));h.push_back(Half(s,0));
 adj[s].push_back(artificial);adj[t].push_back(artificial+1);
 vector<int>nxt(h.size()),face(h.size(),-1);
 for(int u=0;u<n;++u){sort(adj[u].begin(),adj[u].end(),Compare());int d=adj[u].size();for(int j=0;j<d;++j)nxt[adj[u][j]^1]=adj[u][(j+d-1)%d];}
 int faces=0;
 for(int i=0;i<(int)h.size();++i)if(face[i]<0){int k=i;do{face[k]=faces;k=nxt[k];}while(k!=i);++faces;}
 vector<vector<pair<int,double> > >dual(faces);
 for(int i=0;i<artificial;i+=2){int a=face[i],b=face[i^1];if(a!=b){dual[a].push_back(make_pair(b,cap[i/2]));dual[b].push_back(make_pair(a,cap[i/2]));}}
 int start=face[artificial],goal=face[artificial+1];
 vector<double>dist(faces,1e300);dist[start]=0;
 priority_queue<pair<double,int>,vector<pair<double,int> >,greater<pair<double,int> > >q;q.push(make_pair(0.0,start));
 while(!q.empty()){double d=q.top().first;int u=q.top().second;q.pop();if(d!=dist[u])continue;if(u==goal)break;
 for(int j=0;j<(int)dual[u].size();++j){int v=dual[u][j].first;double nd=d+dual[u][j].second;if(nd<dist[v]){dist[v]=nd;q.push(make_pair(nd,v));}}}
 cout<<fixed<<setprecision(4)<<dist[goal]<<'\n';return 0;
}
