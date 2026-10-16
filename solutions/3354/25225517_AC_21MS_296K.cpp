// POJ 3354 - Connect the Dots
// Model: gpt-6.1-sol
// Sort the outgoing half-edges around every supplied vertex. The predecessor
// of the reverse half-edge continues the face on the left. Positive signed
// area identifies bounded faces; the rectangular exterior is not a polygon
// of the subdivision. Collinear supplied edges still count separately.
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;
struct Point { int x,y; Point(int a=0,int b=0):x(a),y(b){} };
struct Edge { int from,to; double angle; Edge(int a,int b,double c):from(a),to(b),angle(c){} };
vector<Edge> e;
bool cmp(int a,int b) { return e[a].angle<e[b].angle; }
int main() {
 int n;
 while(scanf("%d",&n)==1 && n) {
  map<pair<int,int>,int> ids; vector<Point> p;
  vector<vector<int> > adj; e.clear();
  for(int i=0;i<n;++i) {
   int x[2],y[2],v[2]; scanf("%d%d%d%d",x,y,x+1,y+1);
   for(int j=0;j<2;++j) {
    pair<int,int> key=make_pair(x[j],y[j]);
    if(ids.find(key)==ids.end()) { int k=p.size(); ids[key]=k; p.push_back(Point(x[j],y[j])); adj.push_back(vector<int>()); }
    v[j]=ids[key];
   }
   for(int j=0;j<2;++j) {
    adj[v[j]].push_back(e.size());
    e.push_back(Edge(v[j],v[1-j],atan2(double(y[1-j]-y[j]),double(x[1-j]-x[j]))));
   }
  }
  vector<int> pos(e.size()),next(e.size());
  for(unsigned int i=0;i<adj.size();++i) {
   sort(adj[i].begin(),adj[i].end(),cmp);
   for(unsigned int j=0;j<adj[i].size();++j) pos[adj[i][j]]=j;
  }
  for(unsigned int i=0;i<e.size();++i) {
   vector<int>& a=adj[e[i].to]; int k=pos[i^1];
   next[i]=a[(k+int(a.size())-1)%a.size()];
  }
  vector<char> used(e.size(),0); int ans=0;
  for(unsigned int i=0;i<e.size();++i) if(!used[i]) {
   int cur=i,count=0; double area=0;
   do {
    used[cur]=1; ++count; Point a=p[e[cur].from],b=p[e[cur].to];
    area+=double(a.x)*b.y-double(a.y)*b.x; cur=next[cur];
   } while(cur!=int(i));
   if(area>0 && count>ans) ans=count;
  }
  printf("%d\n",ans);
 }
 return 0;
}
