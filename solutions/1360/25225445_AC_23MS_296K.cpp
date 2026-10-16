// POJ 1360 - Blue x Red = Bang
// Model: gpt-6.1-sol
// Enumerate all simple polygonizations, fixing rotation and reversal.
// Bitsets reject red polygonizations intersecting any blue edge.
// The polygons are wire boundaries: nesting is allowed if edges do not meet.
// The discussion clarifies that intersection means intersection of edges.
// Polygons need not be convex; all orientation arithmetic uses 64 bits.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct P { int x,y; };
long long cross(P a,P b,P c) { return (long long)(b.x-a.x)*(c.y-a.y)-(long long)(b.y-a.y)*(c.x-a.x); }
bool on(P a,P b,P c) { return cross(a,b,c)==0 && c.x>=min(a.x,b.x) && c.x<=max(a.x,b.x) && c.y>=min(a.y,b.y) && c.y<=max(a.y,b.y); }
int sign(long long x) { return (x>0)-(x<0); }
bool hit(P a,P b,P c,P d) {
 long long x=cross(a,b,c),y=cross(a,b,d),z=cross(c,d,a),w=cross(c,d,b);
 return (sign(x)*sign(y)<0 && sign(z)*sign(w)<0) || on(a,b,c)||on(a,b,d)||on(c,d,a)||on(c,d,b);
}
struct Edge { int a,b; Edge(int x,int y):a(x),b(y){} };
struct Polys {
 vector<P> p; vector<Edge> edges; int eid[9][9]; vector<vector<int> > polys; vector<int> path; bool used[9];
 void dfs() {
  int n=p.size(),k=path.size();
  if(k==n) {
   if(path[1]>path[n-1]) return;
   for(int i=1;i<n-2;i++) if(hit(p[path[n-1]],p[0],p[path[i]],p[path[i+1]])) return;
   polys.push_back(path); return;
  }
  for(int v=1;v<n;v++) if(!used[v]) {
   bool ok=true; for(int i=0;i<k-2;i++) if(hit(p[path[k-1]],p[v],p[path[i]],p[path[i+1]])) {ok=false;break;}
   if(ok) { used[v]=true;path.push_back(v);dfs();path.pop_back();used[v]=false; }
  }
 }
 void build() { int n=p.size();for(int i=0;i<n;i++) for(int j=i+1;j<n;j++){eid[i][j]=eid[j][i]=edges.size();edges.push_back(Edge(i,j));}
  fill(used,used+9,false);used[0]=true;path.push_back(0);dfs();
 }
};
int main(){int t;scanf("%d",&t);while(t--){int b,r;scanf("%d%d",&b,&r);Polys B,R;P p;
 for(int i=0;i<b;i++){scanf("%d%d",&p.x,&p.y);B.p.push_back(p);}for(int i=0;i<r;i++){scanf("%d%d",&p.x,&p.y);R.p.push_back(p);}B.build();R.build();
 vector<vector<int> > rp;for(size_t j=0;j<R.polys.size();j++)rp.push_back(R.polys[j]);
 int count=rp.size(),words=(count+31)/32;
 vector<vector<unsigned int> > bad(B.edges.size(),vector<unsigned int>(words,0));
 for(size_t e=0;e<B.edges.size();e++)for(int j=0;j<count;j++) {
  bool clash=false;for(int k=0;k<r;k++)if(hit(B.p[B.edges[e].a],B.p[B.edges[e].b],R.p[rp[j][k]],R.p[rp[j][(k+1)%r]])){clash=true;break;}
  if(clash)bad[e][j/32]|=1U<<(j%32);
 }
 bool yes=false;
 for(size_t j=0;j<B.polys.size()&&!yes;j++) {
  const vector<int>&q=B.polys[j];
  for(int w=0;w<words&&!yes;w++){unsigned int z=0;for(int k=0;k<b;k++)z|=bad[B.eid[q[k]][q[(k+1)%b]]][w];
   unsigned int mask=~0U;if(w==words-1 && count%32)mask=(1U<<(count%32))-1;
   if((z&mask)!=mask)yes=true;
  }
 }
 puts(yes?"YES":"NO");
}return 0;}
