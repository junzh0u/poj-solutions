// POJ 1729 - Jack and Jill
// Model: gpt-6-astra
// Widest path in the product of the two walking graphs, with integer buckets.
// A child must move until school, then remains there. The other child's
// house and school are obstacles. Distances are checked at integer minutes.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct Edge { int v; char c; Edge(int a,char b):v(a),c(b){} };
int main() {
 int n,cas=0; char g[30][31];
 const int dr[]={-1,1,0,0}, dc[]={0,0,1,-1};
 const char *dir="NSEW";
 while(scanf("%d",&n)==1 && n) {
  int m=n*n,H=0,h=0,S=0,s=0;
  for(int r=0;r<n;r++) { scanf("%s",g[r]); for(int c=0;c<n;c++) {
   int p=r*n+c; if(g[r][c]=='H')H=p; if(g[r][c]=='h')h=p;
   if(g[r][c]=='S')S=p; if(g[r][c]=='s')s=p;
  }}
  vector<Edge> adj[2][900];
  for(int t=0;t<2;t++)for(int p=0;p<m;p++) {
   if(p==(t?s:S)) {adj[t][p].push_back(Edge(p,0));continue;}
   for(int d=0;d<4;d++) {
    int r=p/n+dr[d],c=p%n+dc[d];
    if(r<0||r>=n||c<0||c>=n)continue;
    char ch=g[r][c];
    if(ch=='*'||ch==(t?'H':'h')||ch==(t?'S':'s'))continue;
    adj[t][p].push_back(Edge(r*n+c,dir[d]));
   }
  }
  vector<int> best(m*m,-1),pre(m*m,-1),dist(m*m);
  vector<char> ca(m*m),cb(m*m);
  for(int a=0;a<m;a++)for(int b=0;b<m;b++) {
   int x=a/n-b/n,y=a%n-b%n;dist[a*m+b]=x*x+y*y;
  }
  int start=H*m+h,goal=S*m+s;
  vector<vector<int> > bucket(2*n*n+1);
  best[start]=dist[start];bucket[best[start]].push_back(start);
  for(int value=best[start];value>=0;value--) {
   while(!bucket[value].empty()) {
    int u=bucket[value].back();bucket[value].pop_back();
    if(best[u]!=value)continue;
    if(u==goal)goto found;
    int a=u/m,b=u%m;
    for(unsigned i=0;i<adj[0][a].size();i++)for(unsigned j=0;j<adj[1][b].size();j++) {
     Edge A=adj[0][a][i],B=adj[1][b][j];int v=A.v*m+B.v,w=min(value,dist[v]);
     if(w<=best[v])continue;
     best[v]=w;pre[v]=u;ca[v]=A.c;cb[v]=B.c;bucket[w].push_back(v);
    }
   }
  }
  found:
  string A,B;
  for(int u=goal;u!=start;u=pre[u]) {if(ca[u])A+=ca[u];if(cb[u])B+=cb[u];}
  reverse(A.begin(),A.end());reverse(B.begin(),B.end());
  if(cas++)puts("");
  printf("%.2f\n%s\n%s\n",sqrt((double)best[goal]),A.c_str(),B.c_str());
 }
 return 0;
}
