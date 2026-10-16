// POJ 3144 - Guarding Zion
// Model: gpt-6.1-sol
// Forest DP conditioned on the nearest selected charge to each subtree root.
// An outside charge is a boundary condition, charged only at its own vertex.
// Coverage is the union of radius-D intervals on each tunnel; equality at D
// is permitted by the stated minimum separation.
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
typedef long long LL;
struct Val { LL cover,cost; Val(LL a=-1,LL b=0):cover(a),cost(b){} };
bool better(Val a,Val b){return a.cover>b.cover||(a.cover==b.cover&&a.cost<b.cost);}
struct Edge {int v,w; Edge(int a,int b):v(a),w(b){}};
vector<Edge> g[305]; vector<int> nodes,sub[305];
LL dis[305][305],cost[305],D; Val dp[305][305]; int par[305];
void distances(int u,int p,int s,LL d){dis[s][u]=d;for(unsigned i=0;i<g[u].size();++i)if(g[u][i].v!=p)distances(g[u][i].v,u,s,d+g[u][i].w);}
void root(int u,int p){par[u]=p;nodes.push_back(u);sub[u].push_back(u);for(unsigned i=0;i<g[u].size();++i){int v=g[u][i].v;if(v==p)continue;root(v,u);sub[u].insert(sub[u].end(),sub[v].begin(),sub[v].end());}}
bool inside(int u,int j){return find(sub[u].begin(),sub[u].end(),j)!=sub[u].end();}
void solve(int u){
 for(unsigned z=0;z<g[u].size();++z)if(g[u][z].v!=par[u])solve(g[u][z].v);
 for(unsigned z=0;z<nodes.size();++z){int j=nodes[z];Val ans(0,j==u?cost[u]:0);
  for(unsigned e=0;e<g[u].size();++e){int v=g[u][e].v,L=g[u][e].w;if(v==par[u])continue;Val best;
   bool forced=inside(v,j);
   for(int t=-1;t<(int)sub[v].size();++t){int k=t<0?j:sub[v][t];if(forced&&k!=j)continue;if(!forced&&k!=j&&(dis[u][k]<dis[u][j]||dis[j][k]<D))continue;
    Val a=dp[v][k];if(a.cover<0)continue;
    LL reach=max(0LL,D-dis[u][j])+max(0LL,D-dis[v][k]);a.cover+=min((LL)L,reach);
    if(better(a,best))best=a;
   }
   if(best.cover<0){ans=Val();break;}ans.cover+=best.cover;ans.cost+=best.cost;
  }dp[u][j]=ans;
 }
}
int main(){int n,m,cas=0;while(cin>>n>>m>>D&&(n||m||D)){for(int i=0;i<n;++i){g[i].clear();sub[i].clear();par[i]=-2;cin>>cost[i];}for(int i=0,a,b,w;i<m;++i){cin>>a>>b>>w;g[a].push_back(Edge(b,w));g[b].push_back(Edge(a,w));}Val total(0,0);for(int r=0;r<n;++r)if(par[r]==-2){nodes.clear();root(r,-1);for(unsigned i=0;i<nodes.size();++i)distances(nodes[i],-1,nodes[i],0);solve(r);Val best(0,0);for(unsigned i=0;i<nodes.size();++i)if(better(dp[r][nodes[i]],best))best=dp[r][nodes[i]];total.cover+=best.cover;total.cost+=best.cost;}cout<<"Case "<<++cas<<": "<<total.cover<<" "<<total.cost<<"\n";}return 0;}
