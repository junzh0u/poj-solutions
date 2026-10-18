// POJ 2966 - A safe way
// Model: gpt-6-astra
// Build the visibility graph of the endpoints and polygon vertices, then
// run Dijkstra. Boundary travel and isolated contacts are legal; interior
// travel is not. After rejecting proper crossings, split at every polygon
// vertex on the segment and test each open piece for interior containment.
// Integer and half-integer coordinates make all geometric predicates exact
// in double: the largest cross products are far below 2^53.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} };
vector<P> p;
double cross(P a,P b,P c) {return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);}
int sign(double x) {return (x>0)-(x<0);}
bool on(P a,P b,P c) {return cross(a,b,c)==0 && (c.x-a.x)*(c.x-b.x)+(c.y-a.y)*(c.y-b.y)<=0;}
bool lessP(P a,P b) {return a.x<b.x || (a.x==b.x && a.y<b.y);}
bool inside(P q) {
 int winding=0,n=p.size();
 for(int i=0;i<n;i++) {
  P a=p[i],b=p[(i+1)%n];
  if(on(a,b,q)) return false;
  double c=cross(a,b,q);
  if(a.y<=q.y && b.y>q.y && c>0) ++winding;
  if(a.y>q.y && b.y<=q.y && c<0) --winding;
 }
 return winding!=0;
}
bool visible(P a,P b) {
 vector<P> cuts;cuts.push_back(a);cuts.push_back(b);
 for(int i=0;i<(int)p.size();i++) {
  P c=p[i],d=p[(i+1)%p.size()];
  if(sign(cross(a,b,c))*sign(cross(a,b,d))<0 &&
     sign(cross(c,d,a))*sign(cross(c,d,b))<0) return false;
  if(on(a,b,c)) cuts.push_back(c);
 }
 sort(cuts.begin(),cuts.end(),lessP);
 for(int i=1;i<(int)cuts.size();i++)
  if(inside(P((cuts[i-1].x+cuts[i].x)/2,(cuts[i-1].y+cuts[i].y)/2))) return false;
 return true;
}
int main() {
 P a,b;int n;
 if(scanf("%lf%lf%lf%lf%d",&a.x,&a.y,&b.x,&b.y,&n)!=5) return 0;
 for(int i=0;i<n;i++){P q;scanf("%lf%lf",&q.x,&q.y);p.push_back(q);}
 vector<P> v=p;v.push_back(a);v.push_back(b);n+=2;
 double g[102][102],dist[102];bool used[102]={false};
 for(int i=0;i<n;i++) {
  dist[i]=1e100;
  for(int j=0;j<=i;j++)
   g[i][j]=g[j][i]=visible(v[i],v[j])?sqrt((v[i].x-v[j].x)*(v[i].x-v[j].x)+(v[i].y-v[j].y)*(v[i].y-v[j].y)):1e100;
 }
 dist[n-2]=0;
 for(int k=0;k<n;k++) {
  int u=-1;for(int i=0;i<n;i++) if(!used[i] && (u<0 || dist[i]<dist[u])) u=i;
  used[u]=true;
  for(int i=0;i<n;i++) dist[i]=min(dist[i],dist[u]+g[u][i]);
 }
 printf("%.4f\n",dist[n-1]);return 0;
}
