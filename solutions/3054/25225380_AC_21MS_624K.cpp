// POJ 3054 - High Spies
// Model: gpt-6.1-sol
// Follow the unique source-destination path, propagating the possible amount
// of source-labelled traffic. At a port, outgoing traffic avoids its own
// incoming branch; all other branches must retain enough traffic to match.
// Zero-capacity branches remain part of the topology. All counts fit int.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Edge { int v, out, in; Edge(int a,int b,int c):v(a),out(b),in(c){} };
int main() {
 int tests; if(scanf("%d",&tests)!=1) return 0;
 while(tests--) {
  int n; scanf("%d",&n); vector<vector<Edge> > g(n+1);
  for(int i=1;i<n;++i) { int a,b,c,d; scanf("%d%d%d%d",&a,&b,&c,&d); g[a].push_back(Edge(b,c,d)); g[b].push_back(Edge(a,d,c)); }
  int s,t; scanf("%d%d",&s,&t);
  vector<int> par(n+1,-1),q; q.push_back(s); par[s]=0;
  for(size_t i=0;i<q.size();++i) { int u=q[i]; for(size_t j=0;j<g[u].size();++j) if(par[g[u][j].v]==-1) { par[g[u][j].v]=u; q.push_back(g[u][j].v); } }
  vector<int> path; for(int u=t;u;u=par[u]) path.push_back(u); reverse(path.begin(),path.end());
  int low=g[s][0].out,high=low;
  for(size_t i=1;i+1<path.size();++i) {
   int u=path[i],before=path[i-1],after=path[i+1],total=0,b=0,c=0,d=0;
   for(size_t j=0;j<g[u].size();++j) { total+=g[u][j].in; if(g[u][j].v==after) { b=g[u][j].out; c=g[u][j].in; } if(g[u][j].v==before) d=g[u][j].out; }
   low=max(0,low+b+max(c,d)-total); high=min(high,b);
   for(size_t j=0;j<g[u].size();++j) if(g[u][j].v!=before && g[u][j].v!=after) high=min(high,total-g[u][j].in-g[u][j].out);
  }
  printf("%d %d\n",low,high);
 }
 return 0;
}
