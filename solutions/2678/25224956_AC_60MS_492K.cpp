// POJ 2678 - Mothy
// Model: gpt-6.1-sol
// Build a visibility graph of endpoints and patch vertices, then run Dijkstra.
// Patch interiors are forbidden; their boundaries, including common edges,
// are legal. Use general simple polygons: the discuss board reports that
// judge polygons need not be convex despite the statement's promise.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double EPS=1e-8, INF=1e100;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} };
P operator+(P a,P b){return P(a.x+b.x,a.y+b.y);}
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
P operator*(P a,double t){return P(a.x*t,a.y*t);}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
int sign(double x){return x>EPS?1:(x<-EPS?-1:0);}
bool on(P p,P a,P b){return sign(cross(b-a,p-a))==0 && dot(p-a,p-b)<=EPS;}
bool inside(P p,const vector<P>& q){
 bool c=false; int n=q.size();
 for(int i=0,j=n-1;i<n;j=i++){
  P a=q[j],b=q[i]; if(on(p,a,b))return false;
  if((a.y>p.y)!=(b.y>p.y)){
   double x=a.x+(b.x-a.x)*(p.y-a.y)/(b.y-a.y);
   if(x>p.x)c=!c;
  }
 }
 return c;
}
bool clear(P a,P b,const vector<vector<P> >& polys){
 P d=b-a; double dd=dot(d,d); if(dd<EPS)return true;
 for(int k=0;k<(int)polys.size();k++){
  const vector<P>& q=polys[k]; vector<double> ts; ts.push_back(0);ts.push_back(1);
  for(int i=0;i<(int)q.size();i++){
   P u=q[i],v=q[(i+1)%q.size()];
   if(sign(cross(d,u-a))*sign(cross(d,v-a))<0 &&
      sign(cross(v-u,a-u))*sign(cross(v-u,b-u))<0)return false;
   if(on(u,a,b))ts.push_back(dot(u-a,d)/dd);
  }
  sort(ts.begin(),ts.end());
  for(int i=1;i<(int)ts.size();i++)if(ts[i]-ts[i-1]>1e-12)
   if(inside(a+d*((ts[i]+ts[i-1])*0.5),q))return false;
 }
 return true;
}
int main(){
 int T; if(scanf("%d",&T)!=1)return 0;
 while(T--){
  int n; P a,b;scanf("%d%lf%lf%lf%lf",&n,&a.x,&a.y,&b.x,&b.y);
  vector<P> pts;pts.push_back(a);pts.push_back(b);vector<vector<P> > polys(n);
  for(int i=0;i<n;i++){int m;scanf("%d",&m);for(int j=0;j<m;j++){P p;scanf("%lf%lf",&p.x,&p.y);polys[i].push_back(p);pts.push_back(p);}}
  int m=pts.size();vector<int> valid(m,1);
  for(int i=0;i<m;i++)for(int j=0;j<n;j++)if(inside(pts[i],polys[j]))valid[i]=0;
  vector<vector<double> > g(m,vector<double>(m,INF));
  for(int i=0;i<m;i++)if(valid[i])for(int j=0;j<i;j++)if(valid[j] && clear(pts[i],pts[j],polys)){
   P d=pts[i]-pts[j];g[i][j]=g[j][i]=sqrt(dot(d,d));
  }
  vector<double> dis(m,INF);vector<int> used(m,0);dis[0]=0;
  if(valid[0] && valid[1])for(int z=0;z<m;z++){
   int v=-1;for(int i=0;i<m;i++)if(!used[i] && (v<0 || dis[i]<dis[v]))v=i;
   if(v<0 || dis[v]>INF/2)break;used[v]=1;
   for(int i=0;i<m;i++)if(dis[i]>dis[v]+g[v][i])dis[i]=dis[v]+g[v][i];
  }
  if(!valid[0] || !valid[1] || dis[1]>INF/2)puts("-1");else printf("%.3f\n",dis[1]);
 }
 return 0;
}
